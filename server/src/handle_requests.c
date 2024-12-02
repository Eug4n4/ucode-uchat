#include "db.h"
#include "server.h"

bool are_fields_valid(cJSON *content, char *fields[], int field_count) {
    for (int i = 0; i < field_count; i++) {
        cJSON *field = cJSON_GetObjectItemCaseSensitive(content, fields[i]);
        if (!cJSON_IsString(field) || strlen(field->valuestring) == 0) {
            return false;
        }
    }
    return true;
}

void handle_registration_request(cJSON *request, t_accepted_client *client) {
    cJSON *content           = cJSON_GetObjectItemCaseSensitive(request, "content");
    char  *required_fields[] = { "username", "display_name", "password" };

    if (!are_fields_valid(content, required_fields, 3)) {
        process_response_type(BAD_REQUEST, client);
        return;
    }

    cJSON *username     = cJSON_GetObjectItemCaseSensitive(content, "username");
    cJSON *display_name = cJSON_GetObjectItemCaseSensitive(content, "display_name");
    cJSON *password     = cJSON_GetObjectItemCaseSensitive(content, "password");

    if (!check_username(username->valuestring) || !check_password(password->valuestring)) {
        process_response_type(FAIL_REGISTRATION, client);
        return;
    }

    t_user *user = db_get_user_by_username(username->valuestring);

    if (user) {
        process_response_type(FAIL_REGISTRATION, client);
        free_user(user);
    } else {
        int new_user_id = -2;
        if (db_add_new_user(username->valuestring, display_name->valuestring, password->valuestring, &new_user_id) == 0) {
            client->is_logged_in = true;
            client->client_id    = new_user_id;
            process_response_type(OK_REGISTRATION, client);
        } else {
            client->is_logged_in = false;
            process_response_type(FAIL_REGISTRATION, client);
        }
    }
}

void handle_login_request(cJSON *request, t_accepted_client *client) {
    cJSON *content           = cJSON_GetObjectItemCaseSensitive(request, "content");
    char  *required_fields[] = { "username", "password" };

    if (!are_fields_valid(content, required_fields, 2)) {
        process_response_type(BAD_REQUEST, client);
        return;
    }

    cJSON *username = cJSON_GetObjectItemCaseSensitive(content, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(content, "password");

    if (!check_username(username->valuestring) || !check_password(password->valuestring)) {
        process_response_type(FAIL_LOGIN, client);
        return;
    }

    t_user        *user     = db_get_user_by_username(username->valuestring);
    unsigned char *hash     = hash_password(password->valuestring, strlen(password->valuestring));
    char          *hex_hash = hash_to_hex(hash);

    if (user && strcmp(username->valuestring, user->username) == 0 && memcmp(hex_hash, user->password, HASH_SIZE) == 0) {
        client->is_logged_in = true;
        client->client_id    = user->id;
        process_response_type(OK_LOGIN, client);
        free_user(user);
    } else {
        client->is_logged_in = false;
        process_response_type(FAIL_LOGIN, client);
    }
    free(hex_hash);
    free(hash);
}

void handle_new_private_chat_request(cJSON *request, t_accepted_client *client) {
    if (client->is_logged_in) {
        cJSON      *content            = cJSON_GetObjectItem(request, "content");
        cJSON      *username           = cJSON_GetObjectItem(content, "username");
        char       *chat_name          = cJSON_GetObjectItem(content, "chat_name")->valuestring;
        const char *target_username    = cJSON_GetStringValue(username);
        int         requesting_user_id = client->client_id;

        char *required_fields[] = { "username" };
        if (!are_fields_valid(content, required_fields, 1)) {
            process_response_type(BAD_REQUEST, client);
            return;
        }

        t_user *target_user = db_get_user_by_username(target_username);
        if (!target_user) {
            process_response_type(FAIL_CREATE_NEW_PRIVATE_CHAT, client);
            return;
        }

        int target_user_id = target_user->id;

        int existing_chat_id = db_check_existing_chat(requesting_user_id, target_user_id);
        if (existing_chat_id > 0) {
            process_response_type(FAIL_CREATE_NEW_PRIVATE_CHAT, client);
        } else {
            int new_chat_id = db_create_chat(chat_name, 0);
            if (new_chat_id > 0) {
                db_link_users_to_chat(new_chat_id, requesting_user_id);
                db_link_users_to_chat(new_chat_id, target_user_id);

                process_response_type(OK_CREATE_NEW_PRIVATE_CHAT, client);
            } else {
                process_response_type(FAIL_CREATE_NEW_PRIVATE_CHAT, client);
            }
        }

        free_user(target_user);
    }
}

void handle_new_group_chat_request(cJSON *request, t_accepted_client *client) {
    cJSON *content     = cJSON_GetObjectItemCaseSensitive(request, "content");
    cJSON *users_array = cJSON_GetObjectItemCaseSensitive(content, "users");
    char  *chat_name   = cJSON_GetObjectItemCaseSensitive(content, "chat_name")->valuestring;

    int chat_id = db_create_chat(chat_name, 1);
    if (chat_id < 0) {
        process_response_type(FAIL_CREATE_NEW_GROUP_CHAT, client);
        logging_format(LOG_ERR, "Failed to create new group chat. DB failed to create new chat\n");
        return;
    }

    int size = cJSON_GetArraySize(users_array);
    for (int i = 0; i < size; i++) {
        cJSON *json_username = cJSON_GetArrayItem(users_array, i);
        char  *username      = cJSON_GetObjectItemCaseSensitive(json_username, "username")->valuestring;

        t_user *user = db_get_user_by_username(username);
        if (user == NULL) {
            process_response_type(FAIL_CREATE_NEW_GROUP_CHAT, client);
            logging_format(LOG_ERR, "Failed to create new group chat. User not found.\n");
            continue;
        }

        if (!db_link_users_to_chat(chat_id, user->id)) {
            process_response_type(FAIL_CREATE_NEW_GROUP_CHAT, client);
            syslog(LOG_ERR, "DB. Failed to link user %s to group chat %d", username, chat_id);
        }

        free_user(user);
    }
    process_response_type(OK_CREATE_NEW_GROUP_CHAT, client);
}

void handle_message_request(cJSON *request, t_accepted_client *client, t_server_state *state) {
    if (!client->is_logged_in) {
        return;
    }

    cJSON      *content = cJSON_GetObjectItem(request, "content");
    int         chat_id = cJSON_GetObjectItem(content, "chat_id")->valueint;
    const char *message = cJSON_GetObjectItem(content, "message")->valuestring;

    if (db_save_message(client->client_id, chat_id, message) == -1) {
        syslog(LOG_ERR, "Error saving message to database");
        process_response_type(FAIL_MESSAGE, client);
        return;
    }

    send_message_to_online_chat_users(chat_id, client, message, state);
    process_response_type(OK_MESSAGE, client);
}

void handle_all_chats_request(t_accepted_client *client) {
    if (client->is_logged_in) {
        t_chats *chats = db_get_all_user_chats(client->client_id);
        generate_all_chats_response(OK_GET_ALL_CHATS, &chats, client);
        free_chats(&chats);
    } else {
        process_response_type(FAIL_GET_ALL_CHATS, client);
    }
}

void handle_get_chat_messages_request(cJSON *request, t_accepted_client *client) {
    if (!client->is_logged_in) {
        generate_get_chat_messages_response(FAIL_GET_CHAT_MESSAGES, NULL, client);
        return;
    }

    cJSON *content = cJSON_GetObjectItem(request, "content");
    int    chat_id = cJSON_GetObjectItem(content, "chat_id")->valueint;

    t_messages *messages = db_get_messages_for_chat(chat_id);

    if (!messages || messages->count == 0) {
        generate_get_chat_messages_response(FAIL_GET_CHAT_MESSAGES, NULL, client);
    } else {
        generate_get_chat_messages_response(OK_GET_CHAT_MESSAGES, messages, client);
    }

    free_messages(&messages);
}

void handle_get_all_users_exclude_request(cJSON *request, t_accepted_client *client) {
    t_user *user = db_get_user_by_id(client->client_id);

    if (user) {
        t_users *users = db_get_all_users_exclude_id(user->id);

        generate_all_users_exclude_response(OK_ALL_USERS_EXCLUDE, &users, client);
        free_users(&users);
        free_user(user);
        return;
    }
    generate_all_users_exclude_response(FAIL_ALL_USERS_EXCLUDE, NULL, client);
    (void)request;
}

void handle_new_chat_request(cJSON *request, t_accepted_client *client) {
    cJSON *content     = cJSON_GetObjectItemCaseSensitive(request, "content");
    cJSON *users_array = cJSON_GetObjectItemCaseSensitive(content, "users");
    char  *chat_name   = cJSON_GetObjectItemCaseSensitive(content, "chat_name")->valuestring;
    int    size        = cJSON_GetArraySize(users_array);

    if (size == 1) {
        cJSON *json_username        = cJSON_GetArrayItem(users_array, 0);
        char  *username             = cJSON_GetObjectItemCaseSensitive(json_username, "username")->valuestring;
        cJSON *private_chat_request = cJSON_CreateObject();
        cJSON *private_chat_content = cJSON_CreateObject();

        cJSON_AddStringToObject(private_chat_content, "username", username);
        cJSON_AddStringToObject(private_chat_content, "chat_name", chat_name);
        cJSON_AddItemToObject(private_chat_request, "content", private_chat_content);
        handle_new_private_chat_request(private_chat_request, client);
        cJSON_Delete(private_chat_request);
    } else if (size > 1) {
        handle_new_group_chat_request(request, client);
    } else {
        process_response_type(FAIL_CREATE_NEW_PRIVATE_CHAT, client);
    }
}

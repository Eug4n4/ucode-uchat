#include "db.h"

void handle_registration_request(cJSON *request, t_accepted_client *client) {
    cJSON *content = cJSON_GetObjectItemCaseSensitive(request, "content");
    cJSON *username = cJSON_GetObjectItemCaseSensitive(content, "username");
    cJSON *display_name = cJSON_GetObjectItemCaseSensitive(content, "display_name");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(content, "password");

    t_user *user = db_get_user_by_username(username->valuestring);

    if (user) {
        process_response_type(FAIL_REGISTRATION, client);
    } else {
        if (db_add_new_user(username->valuestring, display_name->valuestring, password->valuestring) == 0) {
            client->is_logged_in = true;
            client->client_id = user->id;
            process_response_type(OK_REGISTRATION, client);
        } else {
            client->is_logged_in = false;
            process_response_type(FAIL_REGISTRATION, client);
        }
    }
}


void handle_login_request(cJSON *request, t_accepted_client *client) {
    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "content")->child;
    cJSON *password = username->next;
    t_user *user = db_get_user_by_username(username->valuestring);

   if (user) {
        if (strcmp(username->valuestring, user->username) == 0 &&
            strcmp(password->valuestring, user->password) == 0) {
            client->is_logged_in = true;
            client->client_id = user->id;
            process_response_type(OK_LOGIN, client);
            

            return;
            }
    }
    client->is_logged_in = false;
    process_response_type(FAIL_LOGIN, client);
}

void handle_new_private_chat_request(cJSON *request, t_accepted_client *client) {
    if (client->is_logged_in) {
        cJSON *content = cJSON_GetObjectItem(request, "content");
        cJSON *username = cJSON_GetObjectItem(content, "username");

        const char *target_username = cJSON_GetStringValue(username);
        int requesting_user_id = client->client_id;

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
            int new_chat_id = db_create_chat("private_chat", 0);
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

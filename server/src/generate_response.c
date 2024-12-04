#include "db.h"

void generate_login_response(int response, const char *response_message, t_accepted_client *client) {
    cJSON *login_response = cJSON_CreateObject();
    cJSON *content        = cJSON_CreateObject();

    switch (response) {
    case OK_LOGIN:
        cJSON_AddNumberToObject(login_response, "response_type", OK_LOGIN);
        cJSON_AddStringToObject(content, "message", response_message);
        cJSON_AddItemToObject(login_response, "content", content);
        send_response(login_response, client);

        break;
    case FAIL_LOGIN:
        cJSON_AddNumberToObject(login_response, "response_type", FAIL_LOGIN);
        cJSON_AddStringToObject(content, "message", response_message);
        cJSON_AddItemToObject(login_response, "content", content);
        send_response(login_response, client);

        break;
    default:
        break;
    }
    cJSON_Delete(login_response);
}

void generate_registration_response(int response, const char *response_message, t_accepted_client *client) {
    cJSON *registration_response = cJSON_CreateObject();
    cJSON *content               = cJSON_CreateObject();

    switch (response) {
    case OK_REGISTRATION:
        cJSON_AddNumberToObject(registration_response, "response_type", OK_REGISTRATION);
        cJSON_AddStringToObject(content, "message", response_message);
        cJSON_AddItemToObject(registration_response, "content", content);
        send_response(registration_response, client);
        break;

    case FAIL_REGISTRATION:
        cJSON_AddNumberToObject(registration_response, "response_type", FAIL_REGISTRATION);
        cJSON_AddStringToObject(content, "message", response_message);
        cJSON_AddItemToObject(registration_response, "content", content);
        send_response(registration_response, client);
        break;
    default:
        break;
    }
    cJSON_Delete(registration_response);
}

void generate_new_private_chat_response(int response, const char *response_message, t_accepted_client *client) {
    cJSON *new_private_chat_response = cJSON_CreateObject();
    cJSON *content                   = cJSON_CreateObject();

    switch (response) {
    case OK_CREATE_NEW_PRIVATE_CHAT:
        cJSON_AddNumberToObject(new_private_chat_response, "response_type", OK_CREATE_NEW_PRIVATE_CHAT);
        cJSON_AddStringToObject(content, "message", response_message);
        cJSON_AddItemToObject(new_private_chat_response, "content", content);
        send_response(new_private_chat_response, client);
        break;

    case FAIL_CREATE_NEW_PRIVATE_CHAT:
        cJSON_AddNumberToObject(new_private_chat_response, "response_type", FAIL_CREATE_NEW_PRIVATE_CHAT);
        cJSON_AddStringToObject(content, "message", response_message);
        cJSON_AddItemToObject(new_private_chat_response, "content", content);
        send_response(new_private_chat_response, client);
        break;
    default:
        break;
    }
    cJSON_Delete(new_private_chat_response);
}

void generate_new_group_chat_response(int response, const char *response_message, t_accepted_client *client) {
    cJSON *new_group_chat_response = cJSON_CreateObject();
    cJSON *content                 = cJSON_CreateObject();

    switch (response) {
    case OK_CREATE_NEW_GROUP_CHAT:
        cJSON_AddNumberToObject(new_group_chat_response, "response_type", OK_CREATE_NEW_GROUP_CHAT);
        cJSON_AddStringToObject(content, "message", response_message);
        cJSON_AddItemToObject(new_group_chat_response, "content", content);
        send_response(new_group_chat_response, client);
        break;

    case FAIL_CREATE_NEW_GROUP_CHAT:
        cJSON_AddNumberToObject(new_group_chat_response, "response_type", FAIL_CREATE_NEW_GROUP_CHAT);
        cJSON_AddStringToObject(content, "message", response_message);
        cJSON_AddItemToObject(new_group_chat_response, "content", content);
        send_response(new_group_chat_response, client);
        break;

    default:
        logging_format(LOG_ERR, "Failed to send new group chat response.\n");
        break;
    }

    cJSON_Delete(new_group_chat_response);
}

void generate_message_response(int response, t_accepted_client *client) {
    cJSON *message_response = cJSON_CreateObject();
    cJSON *content          = cJSON_CreateObject();

    switch (response) {
    case OK_MESSAGE:
        cJSON_AddNumberToObject(message_response, "response_type", OK_MESSAGE);
        cJSON_AddStringToObject(content, "message", "Message is sent succesfully");
        cJSON_AddItemToObject(message_response, "content", content);
        send_response(message_response, client);
        break;

    case FAIL_MESSAGE:
        cJSON_AddNumberToObject(message_response, "response_type", FAIL_MESSAGE);
        cJSON_AddStringToObject(content, "message", "Failed to send message");
        cJSON_AddItemToObject(message_response, "content", content);
        send_response(message_response, client);
        break;
    default:
        break;
    }
    cJSON_Delete(message_response);
}

void generate_all_chats_response(int response, t_chats **chats, t_accepted_client *client) {
    cJSON *all_chats_response = cJSON_CreateObject();
    cJSON *content            = cJSON_CreateObject();
    cJSON *json_chats         = cJSON_CreateArray();

    switch (response) {
    case OK_GET_ALL_CHATS:
        cJSON_AddNumberToObject(all_chats_response, "response_type", OK_GET_ALL_CHATS);
        t_user  *user = db_get_user_by_id(client->client_id);
        t_chats *head = *chats;

        if (user) {
            cJSON_AddStringToObject(content, "display_name", user->display_name);
        }
        while (head->chat) {
            cJSON *chat         = cJSON_CreateObject();
            int    chat_members = db_get_chat_members_count(head->chat->id);
            cJSON_AddNumberToObject(chat, "chat_id", head->chat->id);
            cJSON_AddNumberToObject(chat, "chat_type", head->chat->type);
            cJSON_AddStringToObject(chat, "chat_name", head->chat->name);
            cJSON_AddNumberToObject(chat, "chat_members", chat_members);

            cJSON_AddItemToArray(json_chats, chat);

            head = head->next;
        }

        cJSON_AddItemToObject(content, "chats", json_chats);
        cJSON_AddItemToObject(all_chats_response, "content", content);

        send_response(all_chats_response, client);
        if (user) {
            free_user(user);
        }
        break;
    case FAIL_GET_ALL_CHATS:
        cJSON_AddNumberToObject(all_chats_response, "response_type", FAIL_GET_ALL_CHATS);
        cJSON_AddStringToObject(content, "message", "User must be logged in before getting all his chats");
        cJSON_AddItemToObject(all_chats_response, "content", content);
        send_response(all_chats_response, client);
        break;
    default:
        break;
    }
    cJSON_Delete(all_chats_response);
}

void generate_get_chat_messages_response(int response, t_messages *messages, t_accepted_client *client) {
    cJSON *response_json  = cJSON_CreateObject();
    cJSON *content        = cJSON_CreateObject();
    cJSON *messages_array = cJSON_CreateArray();

    switch (response) {
    case OK_GET_CHAT_MESSAGES:
        cJSON_AddNumberToObject(response_json, "response_type", OK_GET_CHAT_MESSAGES);

        for (t_message *msg = messages->head; msg != NULL; msg = msg->next) {
            cJSON *message_json = cJSON_CreateObject();
            cJSON_AddNumberToObject(message_json, "id", msg->id);
            cJSON_AddNumberToObject(message_json, "sender_id", msg->sender_id);
            cJSON_AddStringToObject(message_json, "content", msg->content);
            cJSON_AddNumberToObject(message_json, "timestamp", msg->timestamp);
            cJSON_AddItemToArray(messages_array, message_json);
        }

        cJSON_AddItemToObject(content, "messages", messages_array);
        cJSON_AddItemToObject(response_json, "content", content);
        break;

    case FAIL_GET_CHAT_MESSAGES:
        cJSON_AddNumberToObject(response_json, "response_type", FAIL_GET_CHAT_MESSAGES);
        cJSON_AddStringToObject(content, "message", "Failed to retrieve chat messages. Please check the chat ID or try again later.");
        cJSON_AddItemToObject(response_json, "content", content);
        break;

    default:
        cJSON_AddNumberToObject(response_json, "response_type", FAIL_GET_CHAT_MESSAGES);
        cJSON_AddStringToObject(content, "message", "An unknown error occurred.");
        cJSON_AddItemToObject(response_json, "content", content);
        break;
    }

    send_response(response_json, client);

    cJSON_Delete(response_json);
}

void generate_all_users_exclude_response(int response, t_users **users, t_accepted_client *client) {
    cJSON *all_users_response = cJSON_CreateObject();
    cJSON *content            = cJSON_CreateObject();
    cJSON *json_users         = cJSON_CreateArray();

    switch (response) {
    case OK_ALL_USERS_EXCLUDE:
        cJSON_AddNumberToObject(all_users_response, "response_type", OK_ALL_USERS_EXCLUDE);
        t_users *head = *users;

        while (head->user) {
            cJSON *user = cJSON_CreateObject();
            cJSON_AddStringToObject(user, "username", head->user->username);
            cJSON_AddItemToArray(json_users, user);

            head = head->next;
        }
        cJSON_AddItemToObject(content, "users", json_users);
        cJSON_AddItemToObject(all_users_response, "content", content);
        break;
    case FAIL_ALL_USERS_EXCLUDE:
        cJSON_AddNumberToObject(all_users_response, "response_type", FAIL_ALL_USERS_EXCLUDE);
        break;
    default:
        break;
    }
    send_response(all_users_response, client);
    cJSON_Delete(all_users_response);
}

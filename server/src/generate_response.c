#include "db.h"

void generate_login_response(int response, t_accepted_client *client) {
    cJSON *login_response = cJSON_CreateObject();
    cJSON *content = cJSON_CreateObject();

    switch (response) {
    case OK_LOGIN:
        cJSON_AddNumberToObject(login_response, "response_type", OK_LOGIN);
        cJSON_AddStringToObject(content, "message", "Login success");
        cJSON_AddItemToObject(login_response, "content", content);
        send_response(login_response, client);

        break;
    case FAIL_LOGIN:
        cJSON_AddNumberToObject(login_response, "response_type", FAIL_LOGIN);
        cJSON_AddStringToObject(content, "message", "Login failed");
        cJSON_AddItemToObject(login_response, "content", content);
        send_response(login_response, client);

        break;
    default:
        break;
    }
}

void generate_registration_response(int response, t_accepted_client *client) {
    cJSON *registration_response = cJSON_CreateObject();
    cJSON *content = cJSON_CreateObject();

    switch (response) {
    case OK_REGISTRATION:
        cJSON_AddNumberToObject(registration_response, "response_type", OK_REGISTRATION);
        cJSON_AddStringToObject(content, "message", "Registration success");
        cJSON_AddItemToObject(registration_response, "content", content);
        send_response(registration_response, client);
        break;

    case FAIL_REGISTRATION:
        cJSON_AddNumberToObject(registration_response, "response_type", FAIL_REGISTRATION);
        cJSON_AddStringToObject(content, "message", "Registration failed");
        cJSON_AddItemToObject(registration_response, "content", content);
        send_response(registration_response, client);
        break;
    default:
        break;
    }
}

void generate_new_private_chat_response(int response, t_accepted_client *client) {
    cJSON *new_private_chat_response = cJSON_CreateObject();
    cJSON *content = cJSON_CreateObject();

    switch (response) {
    case OK_CREATE_NEW_PRIVATE_CHAT:
        cJSON_AddNumberToObject(new_private_chat_response, "response_type", OK_CREATE_NEW_PRIVATE_CHAT);
        cJSON_AddStringToObject(content, "message", "New private chat is created");
        cJSON_AddItemToObject(new_private_chat_response, "content", content);
        send_response(new_private_chat_response, client);
        break;

    case FAIL_CREATE_NEW_PRIVATE_CHAT:
        cJSON_AddNumberToObject(new_private_chat_response, "response_type", FAIL_CREATE_NEW_PRIVATE_CHAT);
        cJSON_AddStringToObject(content, "message", "Failed to create new private chat");
        cJSON_AddItemToObject(new_private_chat_response, "content", content);
        send_response(new_private_chat_response, client);
        break;
    default:
        break;
    }
}

void generate_message_response(int response, t_accepted_client *client) {
    cJSON *message_response = cJSON_CreateObject();
    cJSON *content = cJSON_CreateObject();

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
}

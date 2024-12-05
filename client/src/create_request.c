#include "../inc/client.h"

cJSON *create_request_registration(void) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, KEY_REQUEST_TYPE, 0);
    cJSON *content = cJSON_CreateObject();
    cJSON_AddStringToObject(content, KEY_USERNAME, "");
    cJSON_AddStringToObject(content, KEY_DISPLAY_NAME, "");
    cJSON_AddStringToObject(content, KEY_PASSWORD, "");
    cJSON_AddItemToObject(json, KEY_CONTENT, content);
    return json;
}

cJSON *create_request_login(void) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, KEY_REQUEST_TYPE, 1);
    cJSON *content = cJSON_CreateObject();
    cJSON_AddStringToObject(content, KEY_USERNAME, "");
    cJSON_AddStringToObject(content, KEY_PASSWORD, "");
    cJSON_AddItemToObject(json, KEY_CONTENT, content);
    return json;
}

cJSON *create_request_new_private_chat(void) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, KEY_REQUEST_TYPE, CREATE_NEW_PRIVATE_CHAT);
    cJSON *content = cJSON_CreateObject();
    cJSON_AddStringToObject(content, KEY_USERNAME, "");
    cJSON_AddItemToObject(json, KEY_CONTENT, content);
    return json;
}

cJSON *create_request_all_user_chats(void) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, KEY_REQUEST_TYPE, GET_ALL_CHATS);
    cJSON *content = cJSON_CreateObject();
    cJSON_AddItemToObject(json, KEY_CONTENT, content);
    return json;
}

cJSON *create_request_all_users_exclude(void) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, KEY_REQUEST_TYPE, ALL_USERS_EXCLUDE);
    cJSON *content = cJSON_CreateObject();
    cJSON_AddItemToObject(json, KEY_CONTENT, content);
    return json;
}

cJSON *create_request_new_chat(void) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, KEY_REQUEST_TYPE, NEW_CHAT);
    cJSON *content = cJSON_CreateObject();
    cJSON_AddItemToObject(json, KEY_CONTENT, content);
    return json;
}

cJSON *create_request_message() {
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, KEY_REQUEST_TYPE, MESSAGE);
    cJSON *content = cJSON_CreateObject();

    cJSON_AddNumberToObject(content, KEY_CHAT_ID, 0);
    cJSON_AddStringToObject(content, KEY_MESSAGE, "");

    cJSON_AddItemToObject(json, KEY_CONTENT, content);

    return json;
}

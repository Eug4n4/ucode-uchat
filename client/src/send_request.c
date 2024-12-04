#include "../inc/client.h"

int send_login_request(const gchar *username, const gchar *password, SSL *ssl) {
    cJSON *request = create_request_login();
    cJSON *content = cJSON_GetObjectItemCaseSensitive(request, KEY_CONTENT);
    cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_USERNAME, cJSON_CreateString(username));
    cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_PASSWORD, cJSON_CreateString(password));

    char *str = cJSON_PrintUnformatted(request);

    int result = SSL_write(ssl, str, strlen(str));
    free(str);

    cJSON_Delete(request);

    return result;
}

int send_registration_request(const gchar *username, const gchar *password, SSL *ssl) {
    cJSON *request = create_request_registration();
    cJSON *content = cJSON_GetObjectItemCaseSensitive(request, KEY_CONTENT);
    cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_USERNAME, cJSON_CreateString(username));
    cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_DISPLAY_NAME, cJSON_CreateString(username));
    cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_PASSWORD, cJSON_CreateString(password));

    char *str = cJSON_PrintUnformatted(request);

    int result = SSL_write(ssl, str, strlen(str));
    free(str);

    cJSON_Delete(request);

    return result;
}

int send_all_user_chats_request(SSL *ssl) {
    cJSON *request = create_request_all_user_chats();
    char  *str     = cJSON_PrintUnformatted(request);
    int    result  = SSL_write(ssl, str, strlen(str));

    free(str);
    cJSON_Delete(request);

    return result;
}

int send_all_users_exclude_request(t_client_data *client_data) {
    cJSON *request = create_request_all_users_exclude();
    char *request_str = cJSON_PrintUnformatted(request);
    int result = SSL_write(client_data->ssl, request_str, strlen(request_str));

    free(request_str);
    cJSON_Delete(request);

    return result;
}

int send_create_chat_request(t_app *app, const char *chat_name) {
    cJSON *request = create_request_new_chat();
    cJSON *content = cJSON_GetObjectItemCaseSensitive(request, "content");
    cJSON *json_users = cJSON_CreateArray();
    
    cJSON_AddStringToObject(content, "chat_name", chat_name);
    for (t_users *u = app->users; u->user; u = u->next) {
        cJSON *user_data = cJSON_CreateObject();
        cJSON_AddStringToObject(user_data, "username", u->user->username);
        cJSON_AddItemToArray(json_users, user_data);
    }
    cJSON_AddItemToObject(content, "users", json_users);
    char *request_str = cJSON_PrintUnformatted(request);
    int result = SSL_write(client_data->ssl, request_str, strlen(request_str));

    free_users(&app->users);
    free(request_str);
    cJSON_Delete(request);
    return result;
}

int send_get_chat_messages_request(int chat_id) {
    cJSON *request = create_get_chat_messages_request();
    cJSON *content = cJSON_GetObjectItemCaseSensitive(request, "content");
    
    cJSON_AddNumberToObject(content, "chat_id", chat_id);
    char *request_str = cJSON_PrintUnformatted(request);
    int result = SSL_write(client_data->ssl, request_str, strlen(request_str));
    
    free(request_str);
    cJSON_Delete(request);
    return result;
}


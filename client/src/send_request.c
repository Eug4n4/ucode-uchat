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
    cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_PASSWORD, cJSON_CreateString(password));

    char *str = cJSON_PrintUnformatted(request);

    int result = SSL_write(ssl, str, strlen(str));
    free(str);

    cJSON_Delete(request);

    return result;
}

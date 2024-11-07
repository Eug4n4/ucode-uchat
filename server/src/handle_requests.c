#include "db.h"

void handle_registration_request(cJSON *request, t_accepted_client *client) {
    cJSON *content = cJSON_GetObjectItemCaseSensitive(request, "content");
    cJSON *email = cJSON_GetObjectItemCaseSensitive(content, "email");
    cJSON *display_name = cJSON_GetObjectItemCaseSensitive(content, "display_name");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(content, "password");

    t_user *user = db_get_user_by_username(email->valuestring);

    if (user) {
        process_response_type(FAIL_REGISTRATION, client);
    } else {
        if (db_add_new_user(email->valuestring, display_name->valuestring, password->valuestring) == 0) {
            process_response_type(OK_REGISTRATION, client);
        } else {
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
            process_response_type(OK_LOGIN, client);
            return;
            }
    }
    process_response_type(FAIL_LOGIN, client);
}


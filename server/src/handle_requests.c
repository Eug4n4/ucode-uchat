#include "db.h"

void handle_login_request(cJSON *request, t_accepted_client *client) {
    cJSON *login = cJSON_GetObjectItemCaseSensitive(request, "content")->child;
    cJSON *password = login->next;
    t_user *user = db_get_user_by_login(login->valuestring);

    if (user) {
        if (strcmp(login->valuestring, user->login) == 0 &&
            strcmp(password->valuestring, user->password) == 0) {
            process_response_type(OK_LOGIN, client);

            return;
            }
    }
    process_response_type(FAIL_LOGIN, client);
}


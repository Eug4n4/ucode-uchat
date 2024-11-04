#include "db.h"

void generate_login_response(int response, t_accepted_client *client) {
    cJSON *login_response = cJSON_CreateObject();
    cJSON *content = cJSON_CreateObject();

    switch (response) {
    case OK_LOGIN:
        cJSON_AddNumberToObject(login_response, "response_type", OK_LOGIN);
        cJSON_AddStringToObject(content, "message", "Login success");
        cJSON_AddItemToObject(login_response, "content", content);
        send_login_response(login_response, client);

        break;
    case FAIL_LOGIN:
        cJSON_AddNumberToObject(login_response, "response_type", FAIL_LOGIN);
        cJSON_AddStringToObject(content, "message", "Login failed");
        cJSON_AddItemToObject(login_response, "content", content);
        send_login_response(login_response, client);

        break;
    default:
        break;
    }
}



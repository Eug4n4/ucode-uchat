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

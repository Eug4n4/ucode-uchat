#include "db.h"

void process_request_type(cJSON *request, t_accepted_client *client) {
    cJSON *req_type = cJSON_GetObjectItemCaseSensitive(request, "request_type");

    switch (req_type->valueint) {
    case REGISTRATION:
        // TODO handle registration request
        break;
    case LOGIN:
        handle_login_request(request, client);
        break;
    default:
        break;
    }
}


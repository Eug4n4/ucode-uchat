#include "db.h"

void process_request_type(cJSON *request, t_accepted_client *client, t_server_state *state) {
    cJSON *req_type = cJSON_GetObjectItemCaseSensitive(request, "request_type");

    switch (req_type->valueint) {
    case REGISTRATION:
        handle_registration_request(request, client);
        break;
    case LOGIN:
        handle_login_request(request, client);
        break;
    case CREATE_NEW_PRIVATE_CHAT:
        handle_new_private_chat_request(request, client);
        break;
    case MESSAGE:
        handle_message_request(request, client, state);
        break;
    default:
        break;
    }
}

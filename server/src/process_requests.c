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
    case NEW_CHAT:
        handle_new_chat_request(state, request, client);
        break;
    case MESSAGE:
        handle_message_request(request, client, state);
        break;
    case GET_ALL_CHATS:
        handle_all_chats_request(client);
        break;
    case GET_CHAT_MESSAGES:
        handle_get_chat_messages_request(request, client);
        break;
    case ALL_USERS_EXCLUDE:
        handle_get_all_users_exclude_request(request, client);
        break;
    default:
        break;
    }
}

#include "db.h"

void process_response_type(int response_type, t_accepted_client *client) {
    switch (response_type) {
    case OK_LOGIN:
        generate_login_response(OK_LOGIN, client);
        break;
    case FAIL_LOGIN:
        generate_login_response(FAIL_LOGIN, client);
        break;
    case OK_REGISTRATION:
        generate_registration_response(OK_REGISTRATION, client);
        break;
    case FAIL_REGISTRATION:
        generate_registration_response(FAIL_REGISTRATION, client);
        break;
    case OK_CREATE_NEW_PRIVATE_CHAT:
        generate_new_private_chat_response(OK_CREATE_NEW_PRIVATE_CHAT, client);
        break;
    case FAIL_CREATE_NEW_PRIVATE_CHAT:
        generate_new_private_chat_response(FAIL_CREATE_NEW_PRIVATE_CHAT, client);
        break;
    case OK_MESSAGE:
        generate_message_response(OK_MESSAGE, client);
        break;
    case FAIL_MESSAGE:
        generate_message_response(FAIL_MESSAGE, client);
        break;
    default:
        break;
    }
}

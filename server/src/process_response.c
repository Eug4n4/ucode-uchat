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
        //TODO generate registration response
            break;
    default:
        break;
    }
}



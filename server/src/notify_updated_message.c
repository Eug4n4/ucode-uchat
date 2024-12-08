#include "server.h"

void notify_updated_message(t_server_state *state, t_message *message) {
    pthread_mutex_lock(&state->client_list_mutex);
    t_client_node *current = state->client_list_head;

    while (current != NULL) {
        t_accepted_client *c = current->client;

        if (c->is_logged_in) {
            generate_update_message_response(OK_UPDATE_MESSAGE, message, c);
        }
        current = current->next;
    }
    pthread_mutex_unlock(&state->client_list_mutex);

}



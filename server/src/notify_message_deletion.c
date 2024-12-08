#include "server.h"


void notify_message_deletion(t_server_state *state, t_message *deleted_message) {
    pthread_mutex_lock(&state->client_list_mutex);
    t_client_node *current = state->client_list_head;

    while (current != NULL) {
        t_accepted_client *c = current->client;

        if (c->is_logged_in) {
            generate_delete_message_response(OK_DELETE_MESSAGE, deleted_message, c);
        }
        current = current->next;
    }
    pthread_mutex_unlock(&state->client_list_mutex);

}


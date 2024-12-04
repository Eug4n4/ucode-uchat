#include "db.h"

void notify_new_chat_creation(t_server_state *state, t_accepted_client *client, int chat_id) {
    pthread_mutex_lock(&state->client_list_mutex);
    t_client_node *current = state->client_list_head;

    while (current != NULL) {
        t_accepted_client *c = current->client;

        if (c->is_logged_in && c->client_id != client->client_id && db_check_user_in_chat(c->client_id, chat_id)) {
            process_response_type(OK_CREATE_NEW_PRIVATE_CHAT, "A new private chat has been successfully created", c);
        }
        current = current->next;
    }
    pthread_mutex_unlock(&state->client_list_mutex);

}


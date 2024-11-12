#include "db.h"

t_client_node *db_get_online_chat_users(t_server_state *state, int chat_id) {
    pthread_mutex_lock(&state->client_list_mutex);
    t_client_node *online_users = NULL;
    t_client_node *current = state->client_list_head;

    while (current != NULL) {
        if (db_check_user_in_chat(current->client->client_id, chat_id)) {
            t_client_node *new_node = malloc(sizeof(t_client_node));
            new_node->client = current->client;
            new_node->next = online_users;
            online_users = new_node;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&state->client_list_mutex);
    return online_users;
}

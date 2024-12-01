#include "server.h"

void add_client(t_server_state *state, t_accepted_client *client) {
    pthread_mutex_lock(&state->client_list_mutex);

    t_client_node *new_node = malloc(sizeof(t_client_node));
    new_node->client = client;

    if (state->client_list_head == NULL) {
        new_node->next = NULL;
        state->client_list_head = new_node;
    } else {
        new_node->next = state->client_list_head;
        state->client_list_head = new_node;
    }

   

    pthread_mutex_unlock(&state->client_list_mutex);
}

void remove_client(t_server_state *state, t_accepted_client *client) {
    pthread_mutex_lock(&state->client_list_mutex);
    t_client_node **curr = &state->client_list_head;
    while (*curr) {
        if ((*curr)->client == client) {
            t_client_node *temp = *curr;
            *curr = (*curr)->next;
            SSL_shutdown(temp->client->ssl);		
            SSL_free(temp->client->ssl);
            free(temp->client);
            free(temp);
            break;
        }
        curr = &(*curr)->next;
    }
    pthread_mutex_unlock(&state->client_list_mutex);
}


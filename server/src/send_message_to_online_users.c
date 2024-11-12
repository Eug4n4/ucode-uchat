#include "server.h"
#include "db.h"

void send_message_to_online_chat_users(int chat_id, t_accepted_client *sender, const char *message, t_server_state *state) {
    pthread_mutex_lock(&state->client_list_mutex);

    t_client_node *current = state->client_list_head;
    while (current != NULL) {
        t_accepted_client *client = current->client;  // seg fault

        if (client->is_logged_in && client->client_id != sender->client_id && db_check_user_in_chat(client->client_id, chat_id)) {
            cJSON *response = cJSON_CreateObject();
            cJSON_AddNumberToObject(response, "response_type", MESSAGE);
            cJSON_AddNumberToObject(response, "sender_id", sender->client_id);
            cJSON_AddStringToObject(response, "message", message);

            send_response(response, client);
            cJSON_Delete(response);
        }

        current = current->next;
    }

    pthread_mutex_unlock(&state->client_list_mutex);
}

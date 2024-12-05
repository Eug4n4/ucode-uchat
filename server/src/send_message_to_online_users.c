#include "server.h"
#include "db.h"

void send_message_to_online_chat_users(int chat_id, t_accepted_client *sender, const char *message, int64_t timestamp, t_server_state *state) {
    pthread_mutex_lock(&state->client_list_mutex);

    t_client_node *current = state->client_list_head;
    while (current != NULL) {
        t_accepted_client *client = current->client;

        if (client->is_logged_in && client->client_id != sender->client_id && db_check_user_in_chat(client->client_id, chat_id)) {
            cJSON *response = cJSON_CreateObject();
            cJSON *content  = cJSON_CreateObject();
            cJSON_AddNumberToObject(response, "response_type", NEW_MESSAGE);
            cJSON_AddItemToObject(response, "content", content);

            cJSON_AddStringToObject(content, "sender_username", db_get_user_by_id(sender->client_id)->username);
            cJSON_AddStringToObject(content, "message", message);
            cJSON_AddNumberToObject(content, "timestamp", timestamp);

            send_response(response, client);
            cJSON_Delete(response);
        }

        current = current->next;
    }

    pthread_mutex_unlock(&state->client_list_mutex);
}

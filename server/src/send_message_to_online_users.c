#include "db.h"

void send_message_to_online_chat_users(int chat_id, t_accepted_client *sender, const char *message, int64_t timestamp, t_server_state *state) {
    pthread_mutex_lock(&state->client_list_mutex);

    t_client_node *current = state->client_list_head;
    while (current != NULL) {
        t_accepted_client *client = current->client;

        if (client->is_logged_in && db_check_user_in_chat(client->client_id, chat_id)) {
            t_message *msg = db_get_last_created_message();
            t_user *sender_user = db_get_user_by_id(sender->client_id);
            cJSON *response = cJSON_CreateObject();
            cJSON *content  = cJSON_CreateObject();
            cJSON_AddNumberToObject(response, "response_type", NEW_MESSAGE);
            cJSON_AddItemToObject(response, "content", content);
            cJSON_AddNumberToObject(content, "chat_id", chat_id);
            cJSON_AddNumberToObject(content, "id", msg->id);
            cJSON_AddStringToObject(content, "sender_username", sender_user->username);
            cJSON_AddStringToObject(content, "content", message);
            cJSON_AddNumberToObject(content, "timestamp", timestamp);


            send_response(response, client);
            free_user(sender_user);
            free_message(msg);
            cJSON_Delete(response);
        }

        current = current->next;
    }

    pthread_mutex_unlock(&state->client_list_mutex);
}


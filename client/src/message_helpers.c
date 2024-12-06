#include "client.h"

t_message *create_message_from_response(cJSON *json_message) {
    int message_id = cJSON_GetObjectItemCaseSensitive(json_message, "id")->valueint;
    char *sender = cJSON_GetObjectItemCaseSensitive(json_message, "sender_username")->valuestring;
    char *content = cJSON_GetObjectItemCaseSensitive(json_message, "content")->valuestring;
    int64_t timestamp = cJSON_GetObjectItemCaseSensitive(json_message, "timestamp")->valueint;
    t_message *message = malloc(sizeof(t_message));
    
    message->content = mx_strdup(content);
    message->id = message_id;
    message->sender_username = mx_strdup(sender);
    message->timestamp = timestamp;
    
    return message;
}

void free_message(t_message *message) {
    free(message->content);
    free(message->sender_username);
    free(message);
}

void free_messages(void) {
    for (GList *head = client_data->messages_list; head; head = head->next) {
        free_message(head->data);
    }
    if (client_data->messages_list) {
        g_list_free(client_data->messages_list);
        client_data->messages_list = NULL;
    }

}


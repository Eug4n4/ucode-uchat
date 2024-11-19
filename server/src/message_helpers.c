#include "server.h"

t_messages *create_message_list() {
    t_messages *messages = malloc(sizeof(t_messages));
    if (messages) {
        messages->head  = NULL;
        messages->tail  = NULL;
        messages->count = 0;
    }
    return messages;
}

void append_message_to_list(t_messages *messages, t_message *msg) {
    if (!messages || !msg)
        return;

    if (!messages->head) {
        messages->head = msg;
    } else {
        messages->tail->next = msg;
    }

    messages->tail = msg;
    messages->count++;
}

void free_messages(t_messages **messages) {
    if (!messages || !*messages)
        return;

    t_message *current = (*messages)->head;
    while (current) {
        t_message *next = current->next;
        free(current->content);
        free(current);
        current = next;
    }

    free(*messages);
    *messages = NULL;
}

#include "server.h"


t_chat *create_chat(int id, char *name, int type) {
    t_chat *chat = malloc(sizeof(t_chat));
    chat->id = id;
    chat->name = name;
    chat->type = type;

    return chat;
}


t_chats *create_chats(t_chat *chat) {
    t_chats *node = malloc(sizeof(t_chats));

    node->chat = chat;
    node->next = NULL;
    return node;
}

void free_chat(t_chat *chat) {
    if (chat) {
        free(chat->name);
        free(chat);
    }
}

void pop_chats_front(t_chats **chats) {
    if (chats) {
        if (*chats != NULL) {
            t_chats *temp = *chats;

            *chats = (*chats)->next;
            free_chat(temp->chat);
            free(temp);
        }
    }
}


void free_chats(t_chats **chats) {
    while (*chats) {
        pop_chats_front(chats);
    }
}


void add_chat_front(t_chats **chats, t_chat *chat) {
    if (chats) {
        t_chats *first_node = create_chats(chat);
        first_node->next = *chats;
        *chats = first_node;
    }
}


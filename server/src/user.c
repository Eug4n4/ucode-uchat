#include "server.h"

t_user *create_user(void) {
    t_user *new_user = malloc(sizeof(t_user));

    if (!new_user) {
        return NULL;
    }
    return new_user;
}

t_user *create_user_with_params(int id, char *username, char *password, char *display_name) {
    t_user *user = create_user();
    user->id = id;
    user->username = strdup(username);
    user->password = strdup(password);
    user->display_name = strdup(display_name);
    
    return user;
}

void free_user(t_user *user) {
    free(user->username);
    free(user->password);
    free(user->display_name);
    free(user);
}

t_users *create_users(t_user *user) {
    t_users *users = malloc(sizeof(t_users));

    if (!users) {
        return NULL;
    }
    users->user = user;

    return users;
}


void add_users_front(t_users **users, t_user *user) {
    if (users) {
        t_users *first_node = create_users(user);
        first_node->next = *users;
        *users = first_node;
    }
}

void pop_users_front(t_users **users) {
    if (users) {
        if (*users != NULL) {
            t_users *temp = *users;

            *users = (*users)->next;
            free_user(temp->user);
            free(temp);
        }
    }
}

void free_users(t_users **users) {
    while ((*users)->user) {
        pop_users_front(users);
    }
}


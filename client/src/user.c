#include "gui.h"

t_user *create_user(void) {
    t_user *new_user = malloc(sizeof(t_user));
    new_user->username = NULL;
    new_user->display_name = NULL;
    new_user->password = NULL;
    
    if (!new_user) {
        return NULL;
    }
    return new_user;
}

t_user *create_user_with_params(char *username, char *password, char *display_name) {
    t_user *user = create_user();
    user->username = g_strdup(username);
    user->password = g_strdup(password);
    user->display_name = g_strdup(display_name);

    return user;
}

void free_user(t_user *user) {
    free(user->username);
    if (user->password) {
        free(user->password);
    }
    if (user->display_name) {
        free(user->display_name);
    }
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


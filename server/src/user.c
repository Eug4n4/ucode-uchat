#include "server.h"


t_user *create_user(void) {
    t_user *new_user = malloc(sizeof(t_user));

    if (!new_user) {
        return NULL;
    }
    return new_user;
}


void free_user(t_user *user) {
    free(user->login);
    free(user->password);
    free(user->display_name);
    free(user->first_name);
    free(user->last_name);
    free(user);
}


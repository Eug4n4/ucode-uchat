#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "../inc/client.h"
#define GLADE_LOGIN_PATH "resources/login.glade"
#define GLADE_REGISTER_PATH "resources/registration.glade"


typedef struct s_gtk_sign_up {
    GtkWindow *window;
    GtkBuilder *builder;
    GtkEntry *entry_username;
    GtkEntry *entry_password;
    GtkLabel *label_error;

} t_gtk_sign_up;

typedef struct s_gtk_sign_in {
    GtkWindow *window;
    GtkEntry *entry_username;
    GtkEntry *entry_password;
    GtkLabel *label_error;
    GtkBuilder *builder;
    struct s_gtk_sign_up *gtk_sign_up;
    SSL *ssl;
} t_gtk_sign_in;

void init_gui(int argc, char **argv, t_gtk_sign_in *gtk_sign_in);
t_gtk_sign_in *create_gtk_sign_in_data(void);

#endif //HEADER_H

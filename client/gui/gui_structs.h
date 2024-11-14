#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#define GLADE_LOGIN_PATH "../resources/login.glade"
#define GLADE_REGISTER_PATH "../resources/registration.glade"


typedef struct s_gtk_sign_up {
    GtkWindow *window;
    GtkBuilder *builder;
    GtkEntry *entry_username;
    GtkEntry *entry_password;
    GtkLabel *label_error;

} t_gtk_sign_up;

typedef struct s_gtk_sign_in {
    GtkWindow *window;
    GtkBuilder *builder;
    GtkEntry *entry_username;
    GtkEntry *entry_password;
    GtkLabel *label_error;
    struct s_gtk_sign_up *gtk_sign_up;
} t_gtk_sign_in;

#endif //HEADER_H

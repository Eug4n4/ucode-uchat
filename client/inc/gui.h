#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netinet/in.h>

#define GLADE_LOGIN_PATH "resources/login.glade"
#define GLADE_REGISTER_PATH "resources/registration.glade"

typedef struct s_gtk_sign_up {
    GtkWindow  *window;
    GtkBuilder *builder;
    GtkEntry   *entry_username;
    GtkEntry   *entry_password;
    GtkLabel   *label_error;
} t_gtk_sign_up;

typedef struct s_gtk_sign_in {
    GtkWindow  *window;
    GtkEntry   *entry_username;
    GtkEntry   *entry_password;
    GtkLabel   *label_error;
    GtkBuilder *builder;
    SSL        *ssl;        // SSL connection
    SSL_CTX    *ctx;        // SSL context
    int         server_fd;  // Server socket file descriptor
} t_gtk_sign_in;

extern GtkBuilder    *builder_login;
extern GtkBuilder    *builder_registration;
extern t_gtk_sign_in *gtk_sign_in;
extern t_gtk_sign_up *gtk_sign_up;

void           init_gui(int argc, char **argv, SSL *ssl);
t_gtk_sign_in *create_gtk_sign_in_data(void);
void           sign_up_connect_signals(t_gtk_sign_in *gtk_sign_in, t_gtk_sign_up *gtk_sign_up);

#endif  // GUI_H



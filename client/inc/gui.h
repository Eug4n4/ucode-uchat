#ifndef GUI_H
#define GUI_H
#include <gtk/gtk.h>
#include "common.h"

#define GLADE_LOGIN_PATH "resources/login.glade"
#define GLADE_REGISTER_PATH "resources/registration.glade"
#define GLADE_MAIN_WINDOW_PATH "resources/main.glade"

typedef enum e_screen {
    LOGIN_SCREEN,
    REGISTRATION_SCREEN,
    MAIN_SCREEN
} t_screen;

typedef struct s_gtk_main_window {
    GtkWindow  *window;
    GtkBuilder *builder;
    
} t_gtk_main_window;

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
extern t_gtk_main_window *gtk_main_window;
extern GtkBuilder *builder_main_window;


void           init_gui(int argc, char **argv, SSL *ssl);
t_gtk_sign_in *create_gtk_sign_in_data(void);
t_gtk_sign_up *create_gtk_sign_up_data(void);
t_gtk_main_window *create_gtk_main_window_data(void);
void           sign_up_connect_signals(t_gtk_sign_in *gtk_sign_in, t_gtk_sign_up *gtk_sign_up);
t_gtk_main_window *create_gtk_main_window_data(void);

void show_screen(int screen);
void destroy_screens(void);

GtkWidget* show_reconnect_popup(const char *host, int port, SSL **ssl, SSL_CTX *ctx);
void close_reconnect_popup(GtkWidget *dialog);
#endif  // GUI_H



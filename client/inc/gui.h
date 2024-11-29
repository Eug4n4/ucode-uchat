#ifndef GUI_H
#define GUI_H
#include <gtk/gtk.h>
#include "common.h"

#define GLADE_LOGIN_PATH "resources/login.glade"
#define GLADE_REGISTER_PATH "resources/registration.glade"
#define GLADE_MAIN_WINDOW_PATH "resources/main.glade"
#define GLADE_CREATE_CHAT_PATH "resources/create_chat.glade"

typedef enum e_screen {
    LOGIN_SCREEN,
    REGISTRATION_SCREEN,
    MAIN_SCREEN,
    CREATE_CHAT_SCREEN,
    ALL_SCREENS
} t_screen;


typedef struct s_user {
    char *username;
    char *display_name;
    char *password;
} t_user;

typedef struct s_users {
    t_user *user;
    struct s_users *next;
} t_users;

typedef struct s_connection {
    SSL *ssl;
    int server_fd;
    // t_user *user;
    char *buffer;
} t_connection;

typedef struct s_app {
    t_connection *connection;
    t_user *current_user;
    t_users *users;
} t_app;

typedef struct s_gtk_create_chat {
    GtkWindow  *window;
    GtkListStore *users_store;
    GtkTreeView *view_users;
    GtkEntry *entry_chat_name;
    GtkTreeSelection *selected_user;
    GtkTreeViewColumn *column_username;
    GtkTreeViewColumn *column_toggle;
    GtkButton *btn_create_chat;
    GtkCellRenderer *text_renderer;
    GtkCellRendererToggle *toggle_renderer;

} t_gtk_create_chat;

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
    // SSL        *ssl;        // SSL connection
    // SSL_CTX    *ctx;        // SSL context
    // int         server_fd;  // Server socket file descriptor
    t_connection *connection;
} t_gtk_sign_in;

extern GtkBuilder    *builder_login;
extern GtkBuilder    *builder_registration;
extern t_gtk_sign_in *gtk_sign_in;
extern t_gtk_sign_up *gtk_sign_up;
extern t_gtk_main_window *gtk_main_window;
extern GtkBuilder *builder_main_window;
extern t_gtk_create_chat *gtk_create_chat;
extern GtkBuilder *builder_create_chat;

void           init_gui(int argc, char **argv, t_app *app);
t_gtk_sign_in *create_gtk_sign_in_data(void);
t_gtk_sign_up *create_gtk_sign_up_data(void);
t_gtk_main_window *create_gtk_main_window_data(void);
t_gtk_create_chat *create_gtk_create_chat_data(void);
void           sign_up_connect_signals(t_gtk_sign_in *gtk_sign_in, t_gtk_sign_up *gtk_sign_up);
t_gtk_main_window *create_gtk_main_window_data(void);

void show_screen(int screen);
void destroy_screens(void);
#endif  // GUI_H



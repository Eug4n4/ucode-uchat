#ifndef GUI_H
#define GUI_H
#include "../../common/inc/common.h"
#include <gtk/gtk.h>


#define GLADE_LOGIN_PATH "resources/login.glade"
#define GLADE_REGISTER_PATH "resources/registration.glade"
#define GLADE_MAIN_WINDOW_PATH "resources/main.glade"
#define GLADE_CREATE_CHAT_PATH "resources/create_chat.glade"
#define PRIVATE_CHAT_IMAGE_PATH "resources/private_chat_icon.png"
#define GROUP_CHAT_IMAGE_PATH "resources/group_icon.png"
#define CSS_STYLE_PATH "./resources/css_style1.css"

typedef enum e_screen {
    LOGIN_SCREEN,
    REGISTRATION_SCREEN,
    MAIN_SCREEN,
    CREATE_CHAT_SCREEN,
    ALL_SCREENS
} t_screen;


typedef struct s_message {
    int               id;
    char             *sender_username;
    char             *content;
    int64_t           timestamp;
} t_message;

typedef struct s_messages {
    t_message *message;
    struct s_messages *next;
} t_messages;

typedef struct s_user {
    char *username;
    char *display_name;
    char *password;
} t_user;

typedef struct s_users {
    t_user         *user;
    struct s_users *next;
} t_users;

typedef struct s_app {
    t_user  *current_user;
    t_users *users;
} t_app;

typedef struct s_gtk_create_chat {
    GtkWindow             *window;
    GtkListStore          *users_store;
    GtkTreeView           *view_users;
    GtkEntry              *entry_chat_name;
    GtkTreeSelection      *selected_user;
    GtkTreeViewColumn     *column_username;
    GtkTreeViewColumn     *column_toggle;
    GtkButton             *btn_create_chat;
    GtkCellRenderer       *text_renderer;
    GtkCellRendererToggle *toggle_renderer;
    GtkLabel              *label_status;

} t_gtk_create_chat;

typedef struct s_gtk_main_window {
    GtkWindow        *window;
    GtkScrolledWindow *chat_history_window;
    GtkListStore     *chat_store;
    GtkPopover *popover_menu;
    GtkTreeView      *chats_list_view;
    GtkTreeSelection *chat_selection;
    GdkPixbuf        *private_chat_image;
    GdkPixbuf        *group_chat_image;
    GtkLabel         *label_chat_name;
    GtkLabel         *label_members_count;
    GtkEntry         *entry_send_message;
    GtkButton        *btn_send_message;
    GtkWidget        *messages;

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
} t_gtk_sign_in;

extern GtkBuilder        *builder_login;
extern GtkBuilder        *builder_registration;
extern t_gtk_sign_in     *gtk_sign_in;
extern t_gtk_sign_up     *gtk_sign_up;
extern t_gtk_main_window *gtk_main_window;
extern GtkBuilder        *builder_main_window;
extern t_gtk_create_chat *gtk_create_chat;
extern GtkBuilder        *builder_create_chat;

void               init_gui(int argc, char **argv, t_app *app);
t_gtk_sign_in     *create_gtk_sign_in_data(void);
t_gtk_sign_up     *create_gtk_sign_up_data(void);
t_gtk_main_window *create_gtk_main_window_data(void);
t_gtk_create_chat *create_gtk_create_chat_data(void);
t_gtk_main_window *create_gtk_main_window_data(void);

void css_set(GtkCssProvider *css_provider, GtkWidget *widget);

void show_screen(int screen);
void destroy_screens(GtkWidget *widget, gpointer data);

GtkWidget *show_reconnect_popup(void);
gboolean   close_reconnect_popup(GtkWidget *dialog);
void on_btn_edit_message_popover_clicked(GtkWidget *button, gpointer data);
gboolean on_message_clicked(GtkWidget *button, GdkEventButton *event, gpointer data);
void destroy_edit_message_buttons(void);
void show_msg_in_chat_history(cJSON *json_message);
gboolean scroll_to_last_message(gpointer data);
void clear_chat_history(void);
#endif  // GUI_H

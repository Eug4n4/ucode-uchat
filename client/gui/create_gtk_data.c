#include "client.h"

t_gtk_main_window *create_gtk_main_window_data(void) {
    t_gtk_main_window *data = malloc(sizeof(t_gtk_main_window));
    GtkWindow         *window;
    GError            *error = NULL;
    // GtkWidget     *log_out_subbtn = NULL;;

    if (!builder_main_window) {
        builder_main_window = gtk_builder_new();
        if (gtk_builder_add_from_file(builder_main_window, GLADE_MAIN_WINDOW_PATH, &error) == 0) {
            g_print("Error loading file %s\n", error->message);
            free(data);
            g_error_free(error);
            return NULL;
        }
    }

    window = GTK_WINDOW(gtk_builder_get_object(builder_main_window, "main_window"));
    // log_out_subbtn = GTK_WIDGET(gtk_builder_get_object(builder_main_window, "log_out_subbtn"));
    data->window  = window;
    data->builder = builder_main_window;

    return data;
}

t_gtk_create_chat *create_gtk_create_chat_data(void) {
    t_gtk_create_chat *data = malloc(sizeof(t_gtk_create_chat));
    GtkWindow  *window;
    GError        *error = NULL;
    GtkListStore *users_store;
    GtkTreeView *view_users;
    GtkEntry *entry_chat_name;
    GtkTreeSelection *selected_user;
    GtkTreeViewColumn *column_username;
    GtkTreeViewColumn *column_toggle;
    GtkButton *btn_create_chat;
    GtkCellRenderer *text_renderer;
    GtkCellRendererToggle *toggle_renderer;


    if (!builder_create_chat) {
        builder_create_chat = gtk_builder_new();
    }
    if (gtk_builder_add_from_file(builder_create_chat, GLADE_CREATE_CHAT_PATH, &error) == 0) {
        g_print("Error loading file %s\n", error->message);
        free(data);
        g_error_free(error);
        return NULL;
    }
    window = GTK_WINDOW(gtk_builder_get_object(builder_create_chat, "window"));
    users_store = GTK_LIST_STORE(gtk_builder_get_object(builder_create_chat, "users_store"));
    view_users = GTK_TREE_VIEW(gtk_builder_get_object(builder_create_chat, "tv_users"));
    selected_user = GTK_TREE_SELECTION(gtk_builder_get_object(builder_create_chat, "selected_user"));
    column_username = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder_create_chat, "column_username"));
    column_toggle = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder_create_chat, "column_toggle"));
    btn_create_chat = GTK_BUTTON(gtk_builder_get_object(builder_create_chat, "btn_create_chat"));
    text_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder_create_chat, "text_renderer"));
    toggle_renderer = GTK_CELL_RENDERER_TOGGLE(gtk_builder_get_object(builder_create_chat, "toggle_renderer"));
    entry_chat_name = GTK_ENTRY(gtk_builder_get_object(builder_create_chat, "entry_chat_name"));

    selected_user = gtk_tree_view_get_selection(view_users);
    
    data->window = window;
    data->view_users = view_users;
    data->selected_user = selected_user;
    data->column_username = column_username;
    data->column_toggle = column_toggle;
    data->btn_create_chat = btn_create_chat;
    data->text_renderer = text_renderer;
    data->toggle_renderer = toggle_renderer;
    data->users_store = users_store;
    data->entry_chat_name = entry_chat_name;
    gtk_tree_view_set_model(data->view_users, GTK_TREE_MODEL(data->users_store)); 
    gtk_tree_view_column_add_attribute(column_username, text_renderer, "text", 0);
    gtk_tree_view_column_add_attribute(column_toggle, GTK_CELL_RENDERER(toggle_renderer), "active", 1);

    return data;
}

t_gtk_sign_up *create_gtk_sign_up_data(void) {
    t_gtk_sign_up *data = malloc(sizeof(t_gtk_sign_up));
    GtkWindow     *window;
    GError        *error = NULL;
    GtkEntry      *entry_username;
    GtkEntry      *entry_password;
    GtkLabel      *label_error;

    if (!builder_registration) {
        builder_registration = gtk_builder_new();
    }

    if (gtk_builder_add_from_file(builder_registration, GLADE_REGISTER_PATH, &error) == 0) {
        g_print("Error loading file %s\n", error->message);
        free(data);
        g_error_free(error);
        return NULL;
    }
    window               = GTK_WINDOW(gtk_builder_get_object(builder_registration, "window"));
    entry_username       = GTK_ENTRY(gtk_builder_get_object(builder_registration, "entry_username"));
    entry_password       = GTK_ENTRY(gtk_builder_get_object(builder_registration, "entry_password"));
    label_error          = GTK_LABEL(gtk_builder_get_object(builder_registration, "label_error"));
    data->window         = window;
    data->entry_username = entry_username;
    data->entry_password = entry_password;
    data->label_error    = label_error;

    return data;
}

t_gtk_sign_in *create_gtk_sign_in_data(void) {
    t_gtk_sign_in *data = malloc(sizeof(t_gtk_sign_in));
    GtkWindow     *window;
    GtkEntry      *entry_username;
    GtkEntry      *entry_password;
    GtkLabel      *label_error;
    GError        *error = NULL;

    if (!builder_login) {
        builder_login = gtk_builder_new();
    }

    if (gtk_builder_add_from_file(builder_login, GLADE_LOGIN_PATH, &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        free(data);
        g_clear_error(&error);
        return NULL;
    }

    window         = GTK_WINDOW(gtk_builder_get_object(builder_login, "window"));
    entry_username = GTK_ENTRY(gtk_builder_get_object(builder_login, "entry_username"));
    entry_password = GTK_ENTRY(gtk_builder_get_object(builder_login, "entry_password"));
    label_error    = GTK_LABEL(gtk_builder_get_object(builder_login, "label_error"));

    data->window         = window;
    data->entry_username = entry_username;
    data->entry_password = entry_password;
    data->label_error    = label_error;

    return data;
}
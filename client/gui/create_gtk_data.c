#include "client.h"

t_gtk_main_window *create_gtk_main_window_data(void) {
    t_gtk_main_window *data = malloc(sizeof(t_gtk_main_window));
    GtkWindow     *window;
    GError        *error = NULL;
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
    
    window         = GTK_WINDOW(gtk_builder_get_object(builder_main_window, "main_window"));
    // log_out_subbtn = GTK_WIDGET(gtk_builder_get_object(builder_main_window, "log_out_subbtn"));
    data->window = window;
    data->builder = builder_main_window;

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



#include "gui.h"
#include "../inc/client.h"
// clang gui.c -o gui `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -Igui_structs.h

GtkBuilder    *builder_login;
GtkBuilder    *builder_registration;
t_gtk_sign_in *gtk_sign_in;
t_gtk_sign_up *gtk_sign_up;

t_gtk_sign_up *create_gtk_sign_up_data() {
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

    gtk_widget_hide(window);
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

void on_btn_sign_in_clicked(GtkButton *button, gpointer data) {
    const gchar *username = gtk_entry_get_text(gtk_sign_in->entry_username);
    const gchar *password = gtk_entry_get_text(gtk_sign_in->entry_password);

    if (strlen(username) == 0 || strlen(password) == 0) {
        gtk_label_set_text(gtk_sign_in->label_error, "Fields cannot be empty");
        return;
    }

    if (send_login_request(username, password, gtk_sign_in->ssl) < 0) {
        gtk_label_set_text(gtk_sign_in->label_error, "Error communicating with server");
        return;
    }
}

void on_btn_sign_up_clicked(GtkButton *button, gpointer data) {
    const gchar *username = gtk_entry_get_text(gtk_sign_up->entry_username);
    const gchar *password = gtk_entry_get_text(gtk_sign_up->entry_password);

    if (strlen(password) == 0 || strlen(username) == 0) {
        gtk_label_set_text(gtk_sign_up->label_error, "Fields cannot be empty");
    } else {
        if (send_registration_request(username, password, gtk_sign_in->ssl) < 0) {
            gtk_label_set_text(gtk_sign_up->label_error, "Error communicating with server");
            return;
        }

        gtk_entry_set_text(gtk_sign_up->entry_username, "");
        gtk_entry_set_text(gtk_sign_up->entry_password, "");
    }

    g_print("Sign up button clicked\n");
}

void on_btn_sign_up_small_clicked(GtkButton *button, gpointer data) {
    if (gtk_sign_in && gtk_sign_in->window) {
        gtk_widget_hide(GTK_WIDGET(gtk_sign_in->window));
    } else {
        g_print("Sign-in window is NULL\n");
    }

    if (!gtk_sign_up) {
        gtk_sign_up = create_gtk_sign_up_data();
        if (!gtk_sign_up) {
            g_print("Failed to create sign-up window\n");
            gtk_main_quit();
            return;
        }
    }

    gtk_widget_show_all(GTK_WIDGET(gtk_sign_up->window));
    g_print("Sign-up button clicked\n");
}

void on_btn_sign_in_small_clicked(GtkButton *button, gpointer data) {
    if (gtk_sign_up && gtk_sign_up->window) {
        gtk_widget_hide(GTK_WIDGET(gtk_sign_up->window));
    } else {
        g_print("Sign-up window is NULL\n");
    }

    if (gtk_sign_in && gtk_sign_in->window) {
        gtk_widget_show_all(GTK_WIDGET(gtk_sign_in->window));
    } else {
        g_print("Sign-in window is NULL\n");
    }
}

void init_gui(int argc, char **argv, SSL *ssl) {
    gtk_init(&argc, &argv);

    gtk_sign_in = create_gtk_sign_in_data();
    if (!gtk_sign_in) {
        printf("Error initializing GUI\n");
        exit(1);
    }
    gtk_sign_in->ssl = ssl;

    gtk_sign_up = create_gtk_sign_up_data();

    GtkButton *btn_sign_in       = GTK_BUTTON(gtk_builder_get_object(builder_login, "btn_sign_in"));
    GtkButton *btn_sign_up_small = GTK_BUTTON(gtk_builder_get_object(builder_login, "btn_sign_up_small"));
    g_signal_connect(btn_sign_in, "clicked", G_CALLBACK(on_btn_sign_in_clicked), NULL);
    g_signal_connect(btn_sign_up_small, "clicked", G_CALLBACK(on_btn_sign_up_small_clicked), NULL);

    GtkButton *btn_sign_up       = GTK_BUTTON(gtk_builder_get_object(builder_registration, "btn_sign_up"));
    GtkButton *btn_sign_in_small = GTK_BUTTON(gtk_builder_get_object(builder_registration, "btn_sign_in_small"));
    g_signal_connect(btn_sign_up, "clicked", G_CALLBACK(on_btn_sign_up_clicked), NULL);
    g_signal_connect(btn_sign_in_small, "clicked", G_CALLBACK(on_btn_sign_in_small_clicked), NULL);

    g_signal_connect(gtk_sign_in->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(gtk_sign_up->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

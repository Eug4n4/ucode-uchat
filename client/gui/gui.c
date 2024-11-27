// #include "gui.h"
#include "../inc/client.h"

GtkBuilder    *builder_login = NULL;
GtkBuilder    *builder_registration = NULL;
t_gtk_sign_in *gtk_sign_in = NULL;
t_gtk_sign_up *gtk_sign_up = NULL;
t_gtk_main_window *gtk_main_window = NULL;
GtkBuilder *builder_main_window = NULL;


void on_btn_sign_in_clicked(GtkButton *button, gpointer data) {
    const gchar *username = gtk_entry_get_text(gtk_sign_in->entry_username);
    const gchar *password = gtk_entry_get_text(gtk_sign_in->entry_password);

    if (strlen(username) == 0 || strlen(password) == 0) {
        gtk_label_set_text(gtk_sign_in->label_error, "Fields cannot be empty");
        return;
    }

    if (!check_username(username)) {
        gtk_label_set_text(gtk_sign_in->label_error, "Invalid username format.");
        return;
    }

    if (send_login_request(username, password, gtk_sign_in->ssl) < 0) {
        gtk_label_set_text(gtk_sign_in->label_error, "Error communicating with server");
        return;
    }
    gtk_entry_set_text(gtk_sign_in->entry_username, "");
    gtk_entry_set_text(gtk_sign_in->entry_password, "");
    show_screen(MAIN_SCREEN);
    send_all_user_chats_request(gtk_sign_in->ssl);
    
    (void)button;
    (void)data;
}

void on_btn_sign_up_clicked(GtkButton *button, gpointer data) {
    const gchar *username = gtk_entry_get_text(gtk_sign_up->entry_username);
    const gchar *password = gtk_entry_get_text(gtk_sign_up->entry_password);

    if (strlen(username) == 0 || strlen(password) == 0) {
        gtk_label_set_text(gtk_sign_up->label_error, "Fields cannot be empty");
    } else if (strcmp(username, password) == 0) {  // Check if username and password are equal
        gtk_label_set_text(gtk_sign_up->label_error, "Username and password cannot be the same.");
    } else {
        if (!check_username(username)) {
            gtk_label_set_text(gtk_sign_up->label_error,
                               "Username requirements:\n"
                               "- Must be longer than 5 characters.\n"
                               "- Only Latin letters, numbers, and underscores are allowed.");
        } else if (!check_password(password)) {
            gtk_label_set_text(gtk_sign_up->label_error,
                               "Password requirements:\n"
                               "- Must be at least 8 characters long.\n"
                               "- Must contain at least:\n"
                               "  - One uppercase letter\n"
                               "  - One lowercase letter\n"
                               "  - One digit\n"
                               "  - One special character.");
        } else {
            if (send_registration_request(username, password, gtk_sign_in->ssl) < 0) {
                gtk_label_set_text(gtk_sign_up->label_error, "Error communicating with server");
                return;
            }
            gtk_entry_set_text(gtk_sign_up->entry_username, "");
            gtk_entry_set_text(gtk_sign_up->entry_password, "");
            show_screen(MAIN_SCREEN);
        }
    }
    (void)button;
    (void)data;
    g_print("Sign up button clicked\n");
}


void on_btn_sign_up_small_clicked(GtkButton *button, gpointer data) {
    if (gtk_sign_up && gtk_sign_up->window && gtk_sign_in && gtk_sign_in->window) {
        gtk_entry_set_text(gtk_sign_in->entry_username, "");
        gtk_entry_set_text(gtk_sign_in->entry_password, "");
        gtk_label_set_text(gtk_sign_in->label_error, "");
        show_screen(REGISTRATION_SCREEN);
    } else {
        g_print("Error in on_btn_sign_up_small_clicked\n");
    }
    (void)button;
    (void)data;
}

void on_btn_sign_in_small_clicked(GtkButton *button, gpointer data) {
    if (gtk_sign_up && gtk_sign_up->window && gtk_sign_in && gtk_sign_in->window) {
        gtk_entry_set_text(gtk_sign_up->entry_username, "");
        gtk_entry_set_text(gtk_sign_up->entry_password, "");
        gtk_label_set_text(gtk_sign_up->label_error, "");
        show_screen(LOGIN_SCREEN);
    } else {
        g_print("Error in on_btn_sign_in_small_clicked\n");
    }
    (void)button;
    (void)data;

}

void on_log_out_subbtn_activate(GtkWidget *log_out_subbtn, gpointer data) {
    g_print("Log out button clicked\n");
    show_screen(LOGIN_SCREEN);
    (void)log_out_subbtn;
    (void)data;
}

void init_gui(int argc, char **argv, SSL *ssl) {
    gtk_init(&argc, &argv);

    gtk_sign_in = create_gtk_sign_in_data();
    if (!gtk_sign_in) {
        printf("Error initializing GUI\n");
        exit(EXIT_FAILURE);
    }
    gtk_sign_in->ssl = ssl;

    gtk_sign_up = create_gtk_sign_up_data();
    gtk_main_window = create_gtk_main_window_data();

    GtkButton *btn_sign_in       = GTK_BUTTON(gtk_builder_get_object(builder_login, "btn_sign_in"));
    GtkButton *btn_sign_up_small = GTK_BUTTON(gtk_builder_get_object(builder_login, "btn_sign_up_small"));
    g_signal_connect(btn_sign_in, "clicked", G_CALLBACK(on_btn_sign_in_clicked), NULL);
    g_signal_connect(btn_sign_up_small, "clicked", G_CALLBACK(on_btn_sign_up_small_clicked), NULL);

    GtkButton *btn_sign_up       = GTK_BUTTON(gtk_builder_get_object(builder_registration, "btn_sign_up"));
    GtkButton *btn_sign_in_small = GTK_BUTTON(gtk_builder_get_object(builder_registration, "btn_sign_in_small"));
    g_signal_connect(btn_sign_up, "clicked", G_CALLBACK(on_btn_sign_up_clicked), NULL);
    g_signal_connect(btn_sign_in_small, "clicked", G_CALLBACK(on_btn_sign_in_small_clicked), NULL);

    GtkWidget *log_out_btn = GTK_WIDGET(gtk_builder_get_object(builder_main_window, "log_out_subbtn"));
    g_signal_connect(log_out_btn, "activate", G_CALLBACK(on_log_out_subbtn_activate), NULL);

    g_signal_connect(gtk_sign_in->window, "destroy", G_CALLBACK(destroy_screens), NULL);
    g_signal_connect(gtk_sign_up->window, "destroy", G_CALLBACK(destroy_screens), NULL);
    g_signal_connect(gtk_main_window->window, "destroy", G_CALLBACK(destroy_screens), NULL);

    show_screen(LOGIN_SCREEN);
}


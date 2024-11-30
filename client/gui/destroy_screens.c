#include "gui.h"
#include "client.h"

static void sign_up_window_quit(void) {
    g_print("Sign-up window quit\n");
    g_object_unref(builder_registration);
    free(gtk_sign_up);
    gtk_sign_up = NULL;
}

static void sign_in_window_quit(void) {
    g_print("Sign-in window quit\n");
    g_object_unref(builder_login);
    free(gtk_sign_in);
    gtk_sign_in = NULL;
}

static void main_window_quit(void) {
    g_print("main window quit\n");
    g_object_unref(builder_main_window);
    free(gtk_main_window);
    gtk_main_window = NULL;
}

static void destroy_screen(t_screen screen) {
    switch (screen) {
    case LOGIN_SCREEN:
        sign_in_window_quit();
        break;
    case REGISTRATION_SCREEN:
        sign_up_window_quit();
        break;
    case MAIN_SCREEN:
        main_window_quit();
        break;
    default:
        break;
    }
}

void destroy_screens(GtkWidget *widget, gpointer data) {
    g_print("Closing GUI and stopping threads...\n");

    g_mutex_lock(&client_data->data_mutex);
    client_data->is_running = false;
    g_mutex_unlock(&client_data->data_mutex);

    if (client_data->server_fd >= 0) {
        close(client_data->server_fd);
    }

    gtk_main_quit();
    (void)widget;
    (void)data;
}

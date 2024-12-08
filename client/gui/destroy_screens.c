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
    g_object_unref(gtk_main_window->private_chat_image);
    g_object_unref(gtk_main_window->group_chat_image);
    g_object_unref(builder_main_window);
    gtk_main_window->private_chat_image = NULL;
    gtk_main_window->group_chat_image = NULL;
    free(gtk_main_window);
    gtk_main_window = NULL;
}

static void create_chat_window_quit(void) {
    g_print("create chat window quit\n");
    gtk_widget_destroy(GTK_WIDGET(gtk_create_chat->window));
    g_object_unref(builder_create_chat);
    free(gtk_create_chat);
    free_messages();
    gtk_create_chat = NULL;
    g_hash_table_destroy(client_data->id_button_table);
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
    case CREATE_CHAT_SCREEN:
        create_chat_window_quit();
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

    for (t_screen screen = LOGIN_SCREEN; screen < ALL_SCREENS; ++screen) {
        destroy_screen(screen);
    }

    free_client_data(client_data);

    gtk_main_quit();
    (void)widget;
    (void)data;
}

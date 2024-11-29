#include "gui.h"


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

static void create_chat_window_quit(void) {
    g_print("create chat window quit\n");
    g_object_unref(builder_create_chat);
    free(gtk_create_chat);
    gtk_create_chat = NULL;
}

static void destroy_screen(t_screen screen) {
    switch (screen)
    {
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

void destroy_screens(void) {
    for (t_screen screen = LOGIN_SCREEN; screen <= CREATE_CHAT_SCREEN; ++screen) {
        destroy_screen(screen);
    }
    gtk_main_quit();

}



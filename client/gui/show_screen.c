#include "gui.h"


void show_screen(int screen) {
    switch (screen)
    {
    case LOGIN_SCREEN:
        gtk_widget_show(GTK_WIDGET(gtk_sign_in->window));
        gtk_widget_hide(GTK_WIDGET(gtk_sign_up->window));
        gtk_widget_hide(GTK_WIDGET(gtk_main_window->window));
        break;
    case REGISTRATION_SCREEN:
        gtk_widget_show(GTK_WIDGET(gtk_sign_up->window));
        gtk_widget_hide(GTK_WIDGET(gtk_sign_in->window));
        gtk_widget_hide(GTK_WIDGET(gtk_main_window->window));
        break;
    case MAIN_SCREEN:
        gtk_widget_show(GTK_WIDGET(gtk_main_window->window));

        gtk_widget_hide(GTK_WIDGET(gtk_main_window->label_chat_name));
        gtk_widget_hide(GTK_WIDGET(gtk_main_window->label_members_count));
        gtk_widget_hide(GTK_WIDGET(gtk_main_window->entry_send_message));
        gtk_widget_hide(GTK_WIDGET(gtk_main_window->btn_send_message));
        
        gtk_widget_hide(GTK_WIDGET(gtk_sign_in->window));
        gtk_widget_hide(GTK_WIDGET(gtk_sign_up->window));
        break;
    case CREATE_CHAT_SCREEN:
        gtk_widget_show(GTK_WIDGET(gtk_create_chat->window));
        break;
    default:
        break;
    }
}



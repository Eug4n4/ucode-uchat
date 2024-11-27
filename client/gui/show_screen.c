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
        gtk_widget_hide(GTK_WIDGET(gtk_sign_in->window));
        gtk_widget_hide(GTK_WIDGET(gtk_sign_up->window));
        break;
    default:
        break;
    }
}



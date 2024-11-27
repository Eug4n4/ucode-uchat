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
        GtkWidget *btn_add_member = GTK_WIDGET(gtk_builder_get_object(builder_main_window, "btn_add_member"));
        GtkWidget *entry_send_message = GTK_WIDGET(gtk_builder_get_object(builder_main_window, "entry_send"));
        GtkWidget *btn_send_message = GTK_WIDGET(gtk_builder_get_object(builder_main_window, "btn_send"));

        gtk_widget_hide(btn_add_member);
        gtk_widget_hide(entry_send_message);
        gtk_widget_hide(btn_send_message);

        gtk_widget_hide(GTK_WIDGET(gtk_sign_in->window));
        gtk_widget_hide(GTK_WIDGET(gtk_sign_up->window));
        break;
    default:
        break;
    }
}



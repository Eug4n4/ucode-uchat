#include "client.h"


void on_popover_closed(GtkWidget *widget, gpointer data) {
    g_signal_handlers_disconnect_by_data(gtk_main_window->btn_edit_message, data);
    g_signal_handlers_disconnect_by_data(gtk_main_window->btn_delete_message, data);
    g_signal_handlers_disconnect_by_data(gtk_main_window->popover_menu, data);
    (void)widget;

}

gboolean is_my_message(GtkWidget *button) {
    gboolean result = FALSE;
    if (GTK_IS_BUTTON(button)) {
        GtkWidget *child = gtk_bin_get_child(GTK_BIN(button));
        if (GTK_IS_LABEL(child)) {
            return FALSE;
        }
        GList *children = gtk_container_get_children(GTK_CONTAINER(child));
        const gchar *text = gtk_label_get_text(GTK_LABEL(children->data));
        
        if (strcmp(text, client_data->current_user->username) == 0) {
            result = TRUE;
        }
        g_list_free(children);
    }
    
    return result;
}

void on_btn_cancel_clicked(GtkWidget *button, gpointer data) {

    destroy_edit_message_buttons();
    gtk_widget_show(GTK_WIDGET(gtk_main_window->btn_send_message));
    (void)button;
    (void)data;
}

void on_btn_edit_clicked(GtkWidget *button, gpointer data) {
    if (GTK_IS_BUTTON(data)) {
        GtkWidget *child = gtk_bin_get_child(GTK_BIN(data));

        if (GTK_IS_BOX(child)) {
            GList *children = gtk_container_get_children(GTK_CONTAINER(child));
            const gchar *new_content = gtk_entry_get_text(gtk_main_window->entry_send_message);
            GList *keys = g_hash_table_get_keys(client_data->id_button_table);

            for (GList *head_messsage = keys; head_messsage; head_messsage = head_messsage->next) {
                gpointer value = g_hash_table_lookup(client_data->id_button_table, head_messsage->data);
                if (memcmp(value, data, sizeof(GtkWidget)) == 0) {
                    send_update_message_request(*(int *)head_messsage->data, new_content);
                    break;
                }
            }
            g_list_free(keys);
            g_list_free(children);
        }
        
        
    }
    destroy_edit_message_buttons();
    gtk_widget_show(GTK_WIDGET(gtk_main_window->btn_send_message));

    (void)button;
       

}
void on_btn_delete_message_popover_clicked(GtkWidget *button, gpointer data) {

    if (GTK_IS_BUTTON(data)) {
        GtkWidget *child = gtk_bin_get_child(GTK_BIN(data));

        if (GTK_IS_BOX(child)) {
            GList *children = gtk_container_get_children(GTK_CONTAINER(child));
            GList *keys = g_hash_table_get_keys(client_data->id_button_table);

            for (GList *head_messsage = keys; head_messsage; head_messsage = head_messsage->next) {
                gpointer value = g_hash_table_lookup(client_data->id_button_table, head_messsage->data);
                if (memcmp(value, data, sizeof(GtkWidget)) == 0) {
                    send_delete_message_request(*(int *)head_messsage->data);
                    break;
                }
            }
            g_list_free(keys);
            g_list_free(children);
        }
    
    }
    gtk_popover_popdown(gtk_main_window->popover_menu);
    (void)button;
}

void on_btn_edit_message_popover_clicked(GtkWidget *button, gpointer data) {
    gtk_widget_hide(GTK_WIDGET(gtk_main_window->btn_send_message));
    GtkGrid           *grid_send_message = GTK_GRID(gtk_builder_get_object(builder_main_window, "send_message_grid"));

    if (!is_my_message(data)) {
        return;
    }
    destroy_edit_message_buttons();
    GtkWidget *edit_button = gtk_button_new();
    GtkWidget *cancel_button = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(cancel_button), "Cancel");
    gtk_button_set_label(GTK_BUTTON(edit_button), "Edit");
    gtk_grid_attach(grid_send_message, edit_button, 2, 0, 1, 1);
    gtk_grid_attach(grid_send_message, cancel_button, 2, 1, 1, 1);
    GtkWidget *child = gtk_bin_get_child(GTK_BIN(data));
    GList *children = gtk_container_get_children(GTK_CONTAINER(child));
    const gchar *text = gtk_label_get_text(GTK_LABEL(children->next->data));
    
    gtk_entry_set_text(gtk_main_window->entry_send_message, text);
    g_list_free(children);
    
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(on_btn_cancel_clicked), NULL);
    g_signal_connect(edit_button, "clicked", G_CALLBACK(on_btn_edit_clicked), data);
    gtk_popover_popdown(gtk_main_window->popover_menu);

    gtk_widget_show(edit_button);
    gtk_widget_show(cancel_button);

    (void)button;
}


gboolean on_message_clicked(GtkWidget *button, GdkEventButton *event, gpointer data) {
    if (event->button == GDK_BUTTON_PRIMARY) {
        return FALSE;
    }
    if (!is_my_message(button)) {
        return TRUE;
    }
    gtk_popover_set_relative_to(gtk_main_window->popover_menu, button);
    gtk_popover_popup(gtk_main_window->popover_menu);
    g_signal_connect(gtk_main_window->popover_menu, "closed", G_CALLBACK(on_popover_closed), button);
    g_signal_connect(gtk_main_window->btn_edit_message, "clicked", G_CALLBACK(on_btn_edit_message_popover_clicked), button);
    g_signal_connect(gtk_main_window->btn_delete_message, "clicked", G_CALLBACK(on_btn_delete_message_popover_clicked), button);
    (void)data;
    return TRUE;
}


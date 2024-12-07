#include "client.h"


gboolean is_my_message(GtkWidget *button) {
    gboolean result = FALSE;
    if (GTK_IS_BUTTON(button)) {
        GtkWidget *child = gtk_bin_get_child(GTK_BIN(button));
        if (GTK_IS_LABEL(child)) {
            g_print("%s\n",gtk_label_get_text(GTK_LABEL(child)));
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
    g_print("Here on_btn_cancel_clicked\n");
    // GtkGrid *send_message_grid = GTK_GRID(gtk_builder_get_object(builder_main_window, "send_message_grid"));
    // GList *children = gtk_container_get_children(GTK_CONTAINER(send_message_grid));
    
    // for (GList *head = children; head; head = head->next) {
    //     if (GTK_IS_BUTTON(head->data)) {
    //         const gchar *label = gtk_button_get_label(head->data);
            
    //         if (strcmp(label, "Send") != 0) {
    //             gtk_widget_destroy(head->data);
    //         }
    //     }
    // }
    // g_list_free(children);
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

            for (GList *head = children; head; head = head->next) {
                if (GTK_IS_LABEL(head->data)) {
                    g_print("label = %s\n", gtk_label_get_text(GTK_LABEL(head->data)));
                }
            }
            g_list_free(children);
        }
        
        
    }
    destroy_edit_message_buttons();
    gtk_widget_show(GTK_WIDGET(gtk_main_window->btn_send_message));

    (void)button;
}


void on_btn_edit_message_popover_clicked(GtkWidget *button, gpointer data) {
    gtk_widget_hide(GTK_WIDGET(gtk_main_window->btn_send_message));
    GtkGrid           *grid_send_message = GTK_GRID(gtk_builder_get_object(builder_main_window, "send_message_grid"));

    // GList *grid_children = gtk_container_get_children(GTK_CONTAINER(grid_send_message));
    
    // for (GList *head = grid_children; head; head = head->next) {
    //     if (GTK_IS_BUTTON(head->data)) {
    //         const gchar *label = gtk_button_get_label(head->data);
            
    //         if (strcmp(label, "Send") != 0) {
    //             gtk_widget_destroy(head->data);
    //         }
    //     }
    // }
    // if (grid_children) {
    //     g_list_free(grid_children);
    // }
    destroy_edit_message_buttons();
    
    // gtk_main_window->edit_button = gtk_button_new();
    // gtk_main_window->cancel_button = gtk_button_new();
    GtkWidget *edit_button = gtk_button_new();
    GtkWidget *cancel_button = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(cancel_button), "Cancel");
    gtk_button_set_label(GTK_BUTTON(edit_button), "Edit");
    gtk_grid_attach(grid_send_message, edit_button, 2, 0, 1, 1);
    gtk_grid_attach(grid_send_message, cancel_button, 2, 1, 1, 1);
    if (!is_my_message(data)) {
        return;
    }
    GtkWidget *child = gtk_bin_get_child(GTK_BIN(data));
    GList *children = gtk_container_get_children(GTK_CONTAINER(child));
    const gchar *text = gtk_label_get_text(GTK_LABEL(children->next->data));
    
    gtk_entry_set_text(gtk_main_window->entry_send_message, text);
    g_list_free(children);
    GtkPopover *update_message_window = GTK_POPOVER(gtk_builder_get_object(builder_main_window, "update_message"));
    
    gtk_popover_popdown(update_message_window);
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(on_btn_cancel_clicked), NULL);
    g_signal_connect(edit_button, "clicked", G_CALLBACK(on_btn_edit_clicked), data);

    gtk_widget_show(edit_button);
    gtk_widget_show(cancel_button);

    (void)button;
    (void)data;
}


gboolean on_message_clicked(GtkWidget *button, GdkEventButton *event, gpointer data) {
    if (event->button == 1) {
        return FALSE;
    }
    if (!is_my_message(button)) {
        return TRUE;
    }
    GtkPopover *update_message_window = GTK_POPOVER(gtk_builder_get_object(builder_main_window, "update_message"));
    GtkWidget *btn_edit_message = GTK_WIDGET(gtk_builder_get_object(builder_main_window, "btn_edit_message"));

    gtk_popover_set_relative_to(update_message_window, button);
    gtk_popover_popup(update_message_window);
    g_signal_connect(btn_edit_message, "clicked", G_CALLBACK(on_btn_edit_message_popover_clicked), button);
    (void)data;
    return TRUE;
}


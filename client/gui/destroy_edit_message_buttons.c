#include "client.h"

void destroy_edit_message_buttons(void) {
    GtkGrid *send_message_grid = GTK_GRID(gtk_builder_get_object(builder_main_window, "send_message_grid"));
    GList *children = gtk_container_get_children(GTK_CONTAINER(send_message_grid));
    
    for (GList *head = children; head; head = head->next) {
        if (GTK_IS_BUTTON(head->data)) {
            const gchar *label = gtk_button_get_label(head->data);
                
            if (strcmp(label, "Send") != 0) {
                gtk_widget_destroy(head->data);
            }
        }
    }
    if (children) {
        g_list_free(children);
    }
    gtk_entry_set_text(gtk_main_window->entry_send_message, "");
}


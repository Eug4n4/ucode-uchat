#include "client.h"

gboolean scroll_to_last_message(gpointer data) {
    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(gtk_main_window->chat_history_window);
    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj));
    (void)data;
    return G_SOURCE_REMOVE;
}

void clear_chat_history(void) {
    GList *childs = gtk_container_get_children(GTK_CONTAINER(gtk_main_window->messages));

    for (GList *head = childs; head; head = head->next) {
        if (GTK_IS_WIDGET(head->data)) {
            gtk_widget_destroy(head->data);
        }
    }
    if (childs) {
        g_list_free(childs);
    }
    free_messages();
}

void show_msg_in_chat_history(cJSON *json_message) {
    int id = cJSON_GetObjectItemCaseSensitive(json_message, "id")->valueint;
    int *message_id = g_new0(gint, 1);
    *message_id = id;
    char *sender = cJSON_GetObjectItemCaseSensitive(json_message, "sender_username")->valuestring;
    char *content = cJSON_GetObjectItemCaseSensitive(json_message, "content")->valuestring;
    int64_t timestamp = cJSON_GetObjectItemCaseSensitive(json_message, "timestamp")->valueint;
    GtkWidget *message_button = gtk_button_new();
    GtkWidget *message_button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *label_username = gtk_label_new(NULL);

    if (strcmp(sender, client_data->current_user->username) == 0) {
        gtk_label_set_text(GTK_LABEL(label_username), client_data->current_user->username);
        gtk_widget_set_halign(message_button, GTK_ALIGN_END);
    } else {
        gtk_label_set_text(GTK_LABEL(label_username), sender);
        gtk_widget_set_halign(message_button, GTK_ALIGN_START);
    }
    
    GtkWidget *label_time = gtk_label_new(ctime(&timestamp));
    GtkWidget *label_text = gtk_label_new(content);

    gtk_label_set_xalign(GTK_LABEL(label_text), 0);
    gtk_label_set_line_wrap(GTK_LABEL(label_text), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(label_text), 60);
    gtk_label_set_line_wrap_mode(GTK_LABEL(label_text), PANGO_WRAP_CHAR | PANGO_WRAP_WORD);

    gtk_box_pack_start(GTK_BOX(message_button_box), label_username, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(message_button_box), label_text, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(message_button_box), label_time, FALSE, FALSE, 5);

    gtk_widget_set_halign(label_username, GTK_ALIGN_START);
    gtk_widget_set_halign(label_text, GTK_ALIGN_START);
    gtk_widget_set_halign(label_time, GTK_ALIGN_END);

    gtk_container_add(GTK_CONTAINER(message_button), message_button_box);
    
    gtk_box_pack_start(GTK_BOX(gtk_main_window->messages), message_button, TRUE, TRUE, 10);
    g_signal_connect(message_button, "button-press-event", G_CALLBACK(on_message_clicked), NULL);
    g_hash_table_insert(client_data->id_button_table, message_id, message_button);
    gtk_widget_show_all(message_button);
}


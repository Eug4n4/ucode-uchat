#include "client.h"

void clear_chat_history(void) {
    GList *childs = gtk_container_get_children(GTK_CONTAINER(gtk_main_window->messages));

    for (GList *head = childs; head; head = head->next) {
        if (GTK_IS_WIDGET(head->data)) {
            gtk_widget_destroy(head->data);
        }
    }
}

void show_msg_in_chat_history(cJSON *json_message) {
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

    gtk_widget_set_name((GtkWidget*)message_button, "message");

    GtkCssProvider *css_provider;
    css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, CSS_STYLE_PATH, NULL);
    css_set(css_provider, message_button);
    
    gtk_box_pack_start(GTK_BOX(gtk_main_window->messages), message_button, TRUE, TRUE, 10);
    gtk_widget_show_all(message_button);
}


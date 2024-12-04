#include "client.h"

void handle_login_response(int response_type, cJSON *json_response) {
    switch (response_type) {
    case OK_LOGIN:
        show_screen(MAIN_SCREEN);
        gtk_list_store_clear(gtk_main_window->chat_store);
        send_all_user_chats_request(client_data->ssl);
        g_mutex_lock(&client_data->data_mutex);
        client_data->is_logged_in = true;
        g_mutex_unlock(&client_data->data_mutex);
        break;
    case FAIL_LOGIN: {
        cJSON *content_json = cJSON_GetObjectItemCaseSensitive(json_response, "content");
        if (content_json) {
            cJSON *message_json = cJSON_GetObjectItemCaseSensitive(content_json, "message");
            if (cJSON_IsString(message_json)) {
                const char *message = message_json->valuestring;
                gtk_label_set_text(gtk_sign_in->label_error, message);
            }
        }
        break;
    }

    default:
        break;
    }
}

void handle_registration_response(int response_type, cJSON *json_response) {
    switch (response_type) {
    case OK_REGISTRATION:
        show_screen(MAIN_SCREEN);
        gtk_list_store_clear(gtk_main_window->chat_store);
        send_all_user_chats_request(client_data->ssl);
        g_mutex_lock(&client_data->data_mutex);
        client_data->is_logged_in = true;
        g_mutex_unlock(&client_data->data_mutex);
        break;
    case FAIL_REGISTRATION: {
        cJSON *content_json = cJSON_GetObjectItemCaseSensitive(json_response, "content");
        if (content_json) {
            cJSON *message_json = cJSON_GetObjectItemCaseSensitive(content_json, "message");
            if (cJSON_IsString(message_json)) {
                const char *message = message_json->valuestring;
                gtk_label_set_text(gtk_sign_up->label_error, message);
            }
            break;
        }
    default:
        break;
    }
    }
}

void handle_get_all_user_chats_response(cJSON *response) {
    cJSON        *content            = cJSON_GetObjectItem(response, "content");
    char         *display_name       = cJSON_GetObjectItemCaseSensitive(content, "display_name")->valuestring;
    cJSON        *chats              = cJSON_GetObjectItemCaseSensitive(content, "chats");
    cJSON        *chat               = NULL;
    GtkLabel     *label_display_name = GTK_LABEL(gtk_builder_get_object(builder_main_window, "label_display_name"));
    GtkTreeModel *model              = gtk_tree_view_get_model(gtk_main_window->chats_list_view);

    gtk_label_set_text(label_display_name, display_name);
    if (!model) {
        printf("no model\n");
        return;
    }
    GtkTreeIter iter;

    cJSON_ArrayForEach(chat, chats) {
        cJSON *chat_type    = cJSON_GetObjectItemCaseSensitive(chat, "chat_type");
        cJSON *chat_name    = cJSON_GetObjectItemCaseSensitive(chat, "chat_name");
        int    chat_members = cJSON_GetObjectItemCaseSensitive(chat, "chat_members")->valueint;
        int chat_id = cJSON_GetObjectItemCaseSensitive(chat, "chat_id")->valueint;

        gtk_list_store_append(GTK_LIST_STORE(model), &iter);
        if (chat_type->valueint == 0) {
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, gtk_main_window->private_chat_image, -1);
        } else {
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, gtk_main_window->group_chat_image, -1);
        }
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, 1, chat_name->valuestring, -1);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, 2, chat_members, -1);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, 3, chat_id, -1);

    }
}

void handle_all_users_exclude_response(cJSON *response) {
    cJSON        *content = cJSON_GetObjectItemCaseSensitive(response, "content");
    cJSON        *users   = cJSON_GetObjectItemCaseSensitive(content, "users");
    cJSON        *user    = NULL;
    GtkTreeModel *model   = gtk_tree_view_get_model(gtk_create_chat->view_users);

    if (!model) {
        printf("no model\n");
        return;
    }
    GtkTreeIter iter;
    cJSON_ArrayForEach(user, users) {
        cJSON *username = cJSON_GetObjectItemCaseSensitive(user, "username");

        if (cJSON_IsString(username)) {
            gtk_list_store_append(gtk_create_chat->users_store, &iter);
            gtk_list_store_set(gtk_create_chat->users_store, &iter, 0, username->valuestring, 1, FALSE, -1);
        }
    }
}

// void handle_private_chat_response(void) {
//     gtk_list_store_clear(gtk_main_window->chat_store);
//     send_all_user_chats_request(client_data->ssl);
//     gtk_widget_hide(GTK_WIDGET(gtk_main_window->label_chat_name));
//     gtk_widget_hide(GTK_WIDGET(gtk_main_window->label_members_count));
//     gtk_widget_hide(GTK_WIDGET(gtk_main_window->entry_send_message));
//     gtk_widget_hide(GTK_WIDGET(gtk_main_window->btn_send_message));
// }

// void handle_group_chat_response(void) {
//     gtk_list_store_clear(gtk_main_window->chat_store);
//     send_all_user_chats_request(client_data->ssl);
//     gtk_widget_hide(GTK_WIDGET(gtk_main_window->label_chat_name));
//     gtk_widget_hide(GTK_WIDGET(gtk_main_window->label_members_count));
//     gtk_widget_hide(GTK_WIDGET(gtk_main_window->entry_send_message));
//     gtk_widget_hide(GTK_WIDGET(gtk_main_window->btn_send_message));
// }

void handle_new_chat_response(cJSON *json_response) {
    gtk_list_store_clear(gtk_main_window->chat_store);
    send_all_user_chats_request(client_data->ssl);
    gtk_widget_hide(GTK_WIDGET(gtk_main_window->label_chat_name));
    gtk_widget_hide(GTK_WIDGET(gtk_main_window->label_members_count));
    gtk_widget_hide(GTK_WIDGET(gtk_main_window->entry_send_message));
    gtk_widget_hide(GTK_WIDGET(gtk_main_window->btn_send_message));
    cJSON *content_json = cJSON_GetObjectItemCaseSensitive(json_response, "content");
    if (content_json) {
        cJSON *message_json = cJSON_GetObjectItemCaseSensitive(content_json, "message");
        if (cJSON_IsString(message_json)) {
            const char *message = message_json->valuestring;
            gtk_label_set_text(gtk_create_chat->label_status, message);
        }
    }
}

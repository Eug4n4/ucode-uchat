#include "client.h"

void add_chat_to_list(GtkTreeIter *iter, int chat_type, char *chat_name, int chat_members, int chat_id) {
    if (chat_type == 0) {
        gtk_list_store_set(gtk_main_window->chat_store, iter, 0, gtk_main_window->private_chat_image, -1);
    } else {
        gtk_list_store_set(gtk_main_window->chat_store, iter, 0, gtk_main_window->group_chat_image, -1);
    }
    gtk_list_store_set(gtk_main_window->chat_store, iter, 1, chat_name, -1);
    gtk_list_store_set(gtk_main_window->chat_store, iter, 2, chat_members, -1);
    gtk_list_store_set(gtk_main_window->chat_store, iter, 3, chat_id, -1);

}

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

    gtk_label_set_text(label_display_name, display_name);
    
    GtkTreeIter iter;

    cJSON_ArrayForEach(chat, chats) {
        cJSON *chat_type    = cJSON_GetObjectItemCaseSensitive(chat, "chat_type");
        cJSON *chat_name    = cJSON_GetObjectItemCaseSensitive(chat, "chat_name");
        int    chat_members = cJSON_GetObjectItemCaseSensitive(chat, "chat_members")->valueint;
        int chat_id = cJSON_GetObjectItemCaseSensitive(chat, "chat_id")->valueint;

        gtk_list_store_append(gtk_main_window->chat_store, &iter);
        add_chat_to_list(&iter, chat_type->valueint, chat_name->valuestring, chat_members, chat_id);

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


void handle_new_chat_response(cJSON *json_response) {
    cJSON *content_json = cJSON_GetObjectItemCaseSensitive(json_response, "content");
    char *chat_name = cJSON_GetObjectItemCaseSensitive(content_json, "chat_name")->valuestring;
    int chat_type = cJSON_GetObjectItemCaseSensitive(content_json, "chat_type")->valueint;
    int chat_members = cJSON_GetObjectItemCaseSensitive(content_json, "chat_members")->valueint;
    int chat_id = cJSON_GetObjectItemCaseSensitive(content_json, "chat_id")->valueint;
    GtkTreeIter iter;

    gtk_list_store_insert(gtk_main_window->chat_store, &iter, 0);
    add_chat_to_list(&iter, chat_type, chat_name, chat_members, chat_id);

    if (content_json) {
        cJSON *message_json = cJSON_GetObjectItemCaseSensitive(content_json, "message");
        if (cJSON_IsString(message_json)) {
            const char *message = message_json->valuestring;
            gtk_label_set_text(gtk_create_chat->label_status, message);
        }
    }
}

void handle_get_chat_messages_response(cJSON *response) {
    cJSON *content = cJSON_GetObjectItemCaseSensitive(response, "content");
    cJSON *messages_array = cJSON_GetObjectItemCaseSensitive(content, "messages");
    cJSON *message = NULL;

    cJSON_ArrayForEach(message, messages_array) {
        show_msg_in_chat_history(message);
    }
}

void handle_new_message_response(cJSON *response) {
    cJSON *content = cJSON_GetObjectItemCaseSensitive(response, "content");
    int chat_id = cJSON_GetObjectItemCaseSensitive(content, "chat_id")->valueint;
    int current_chat_id;
    GtkTreeIter iter;
    GtkTreeModel *model = gtk_tree_view_get_model(gtk_main_window->chats_list_view);

    if (gtk_tree_selection_get_selected(gtk_main_window->chat_selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 3, &current_chat_id, -1);
        if (current_chat_id == chat_id) {
            show_msg_in_chat_history(content);
        }
    }
}


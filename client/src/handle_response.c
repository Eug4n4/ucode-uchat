#include "client.h"


void handle_login_response(int response_type) {
    switch (response_type) {
    case OK_LOGIN:
        show_screen(MAIN_SCREEN);
        send_all_user_chats_request(gtk_sign_in->connection->ssl);
        break;
    case FAIL_LOGIN:
        gtk_label_set_text(gtk_sign_in->label_error, "Login failed. Please check your credentials.");
        break;
    default:
        break;
    }
}

void handle_registration_response(int response_type) {
    switch (response_type) {
    case OK_REGISTRATION:
        show_screen(MAIN_SCREEN);
        send_all_user_chats_request(gtk_sign_in->connection->ssl);
        break;
    case FAIL_REGISTRATION:
        gtk_label_set_text(gtk_sign_up->label_error, "Registration failed. Try a different username.");
        break;
    default:
        break;
    }
}

void handle_get_all_user_chats_response(cJSON *response) {
    cJSON *content = cJSON_GetObjectItem(response, "content");
    char *display_name = cJSON_GetObjectItemCaseSensitive(content, "display_name")->valuestring;
    GtkLabel *label_display_name = GTK_LABEL(gtk_builder_get_object(builder_main_window, "label_display_name"));
    
    gtk_label_set_text(label_display_name, display_name);

}

void handle_all_users_exclude_response(cJSON *response) {
    cJSON *content = cJSON_GetObjectItemCaseSensitive(response, "content");
    cJSON *users = cJSON_GetObjectItemCaseSensitive(content, "users");
    cJSON *user = NULL;
    GtkTreeModel *model = gtk_tree_view_get_model(gtk_create_chat->view_users);
    
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


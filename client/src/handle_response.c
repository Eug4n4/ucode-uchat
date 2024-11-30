#include "client.h"

void handle_login_response(int response_type) {
    switch (response_type) {
    case OK_LOGIN:
        show_screen(MAIN_SCREEN);
        send_all_user_chats_request(client_data->ssl);
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
        send_all_user_chats_request(client_data->ssl);
        break;
    case FAIL_REGISTRATION:
        gtk_label_set_text(gtk_sign_up->label_error, "Registration failed. Try a different username.");
        break;
    default:
        break;
    }
}

void handle_get_all_user_chats_response(cJSON *response) {
    cJSON    *content            = cJSON_GetObjectItem(response, "content");
    char     *display_name       = cJSON_GetObjectItemCaseSensitive(content, "display_name")->valuestring;
    GtkLabel *label_display_name = GTK_LABEL(gtk_builder_get_object(builder_main_window, "label_display_name"));

    gtk_label_set_text(label_display_name, display_name);
}

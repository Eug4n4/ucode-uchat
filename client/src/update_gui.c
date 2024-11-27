#include "../inc/client.h"

gboolean update_gui_with_response(gpointer data) {
    const gchar *response = (const gchar *)data;

    cJSON *json_response = cJSON_Parse(response);
    if (!json_response) {
        g_printerr("Invalid JSON response: %s\n", response);
        g_free(data);
        return FALSE;
    }

    cJSON *response_type_json = cJSON_GetObjectItemCaseSensitive(json_response, KEY_RESPONSE_TYPE);
    if (!cJSON_IsNumber(response_type_json)) {
        g_printerr("Missing or invalid response_type in JSON: %s\n", response);
        cJSON_Delete(json_response);
        g_free(data);
        return FALSE;
    }
    char *res = cJSON_Print(json_response);
    t_response_type response_type = (t_response_type)response_type_json->valueint;

    switch (response_type) {
    case OK_LOGIN:
        // gtk_label_set_text(gtk_sign_in->label_error, "Login successful!");
        break;
    case FAIL_LOGIN:
        // gtk_label_set_text(gtk_sign_in->label_error, "Login failed. Please check your credentials.");
        break;
    case OK_REGISTRATION:
        // gtk_label_set_text(gtk_sign_up->label_error, "Registration successful!");
        break;
    case FAIL_REGISTRATION:
        // gtk_label_set_text(gtk_sign_up->label_error, "Registration failed. Try a different username.");
        break;
    case OK_CREATE_NEW_PRIVATE_CHAT:
        // gtk_label_set_text(gtk_sign_in->label_error, "Chat created successfully!");
        break;
    case FAIL_CREATE_NEW_PRIVATE_CHAT:
        // gtk_label_set_text(gtk_sign_in->label_error, "Failed to create a new chat.");
        break;
    case OK_GET_ALL_CHATS:
        
        printf("Ok\n%s\n",res);
        // gtk_label_set_text(gtk_sign_in->label_error, "Chat list retrieved.");
        break;
    case FAIL_GET_ALL_CHATS:
        printf("Fail\n%s\n",res);
        // gtk_label_set_text(gtk_sign_in->label_error, "Failed to retrieve chat list.");
        break;
    case OK_MESSAGE:
        // gtk_label_set_text(gtk_sign_in->label_error, "Message sent successfully.");
        break;
    case FAIL_MESSAGE:
        // gtk_label_set_text(gtk_sign_in->label_error, "Failed to send message.");
        break;
    default:
        g_printerr("Unknown response type: %d\n", response_type);
        break;
    }

    cJSON_Delete(json_response);
    g_free(data);

    return FALSE;
}

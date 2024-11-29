#include "../inc/client.h"

gboolean update_gui_with_response(gpointer data) {
    t_connection *connection = (t_connection *)data;
    const gchar *response = (const gchar *)connection->buffer;

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
    printf("%s\n",res);
    t_response_type response_type = (t_response_type)response_type_json->valueint;

    switch (response_type) {
    case OK_LOGIN:
        handle_login_response(OK_LOGIN);
        break;
    case FAIL_LOGIN:
        handle_login_response(FAIL_LOGIN);
        break;
    case OK_REGISTRATION:
        handle_registration_response(OK_REGISTRATION);
        break;
    case FAIL_REGISTRATION:
        handle_registration_response(FAIL_REGISTRATION);
        break;
    case OK_CREATE_NEW_PRIVATE_CHAT:
        // gtk_label_set_text(gtk_sign_in->label_error, "Chat created successfully!");
        break;
    case FAIL_CREATE_NEW_PRIVATE_CHAT:
        // gtk_label_set_text(gtk_sign_in->label_error, "Failed to create a new chat.");
        break;
    case OK_GET_ALL_CHATS: {
        printf("%s\n",res);
        handle_get_all_user_chats_response(json_response);
        break;
    } 
    case FAIL_GET_ALL_CHATS:  
        // gtk_label_set_text(gtk_sign_in->label_error, "Failed to retrieve chat list.");
        break;
    case OK_MESSAGE:
        // gtk_label_set_text(gtk_sign_in->label_error, "Message sent successfully.");
        break;
    case FAIL_MESSAGE:
        // gtk_label_set_text(gtk_sign_in->label_error, "Failed to send message.");
        break;
    case OK_ALL_USERS_EXCLUDE:
        handle_all_users_exclude_response(json_response);
        break;
    default:
        g_printerr("Unknown response type: %d\n", response_type);
        break;
    }

    cJSON_Delete(json_response);
    free(connection->buffer);

    return FALSE;
}

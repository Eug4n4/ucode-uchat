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

    t_response_type response_type = (t_response_type)response_type_json->valueint;

    switch (response_type) {
    case OK_LOGIN:
        handle_login_response(OK_LOGIN, json_response);
        break;
    case FAIL_LOGIN:
        handle_login_response(FAIL_LOGIN, json_response);
        break;
    case OK_REGISTRATION:
        handle_registration_response(OK_REGISTRATION, json_response);
        break;
    case FAIL_REGISTRATION:
        handle_registration_response(FAIL_REGISTRATION, json_response);
        break;
    case OK_CREATE_NEW_PRIVATE_CHAT:
        handle_new_chat_response(json_response);
        break;
    case FAIL_CREATE_NEW_PRIVATE_CHAT:
        handle_new_chat_response(json_response);
        break;
    case OK_GET_ALL_CHATS: {
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
    case OK_GET_CHAT_MESSAGES:
        handle_get_chat_messages_response(json_response);
        break;
    case OK_ALL_USERS_EXCLUDE:
        handle_all_users_exclude_response(json_response);
        break;
    case OK_CREATE_NEW_GROUP_CHAT:
        handle_new_chat_response(json_response);
        break;
    case FAIL_CREATE_NEW_GROUP_CHAT:
        handle_new_chat_response(json_response);
        break;
    case NEW_MESSAGE:
        handle_new_message_response(json_response);
        break;
    case OK_UPDATE_MESSAGE:
        handle_update_message_response(json_response);
        break;
    case OK_DELETE_MESSAGE:
        handle_delete_message_response(json_response);
        break;
    default:
        break;
    }

    cJSON_Delete(json_response);
    g_free(data);

    return FALSE;
}



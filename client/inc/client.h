#ifndef CLIENT_H
#define CLIENT_H
#include "gui.h"

#define KEY_REQUEST_TYPE "request_type"
#define KEY_USERNAME "username"
#define KEY_DISPLAY_NAME "display_name"
#define KEY_PASSWORD "password"
#define KEY_CONTENT "content"

#define KEY_RESPONSE_TYPE "response_type"
#define KEY_MESSAGE "message"

#define h_addr h_addr_list[0]

int do_connection(const char *host, int port);

cJSON   *create_request_registration();
cJSON   *create_request_login();
cJSON   *create_request_new_private_chat();
gboolean update_gui_with_response(gpointer data);
int      send_login_request(const gchar *username, const gchar *password, SSL *ssl);
int      send_registration_request(const gchar *username, const gchar *password, SSL *ssl);
#endif  // CLIENT_H

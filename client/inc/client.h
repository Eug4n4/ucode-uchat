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
cJSON   *create_request_all_user_chats(void);
cJSON *create_request_all_users_exclude(void);
gboolean update_gui_with_response(gpointer data);
int      send_login_request(const gchar *username, const gchar *password, SSL *ssl);
int      send_registration_request(const gchar *username, const gchar *password, SSL *ssl);
int      send_all_user_chats_request(SSL *ssl);
int send_all_users_exclude_request(t_connection *connection);

void handle_login_response(int response_type);
void handle_registration_response(int response_type);
void handle_get_all_user_chats_response(cJSON *response);
void handle_all_users_exclude_response(cJSON *response);
#endif  // CLIENT_H


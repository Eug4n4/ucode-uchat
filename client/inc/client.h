#ifndef CLIENT_H
#define CLIENT_H
#include "gui.h"

#define KEY_REQUEST_TYPE "request_type"
#define KEY_USERNAME "username"
#define KEY_DISPLAY_NAME "display_name"
#define KEY_PASSWORD "password"
#define KEY_CONTENT "content"
#define KEY_CHAT_ID "chat_id"

#define KEY_RESPONSE_TYPE "response_type"
#define KEY_MESSAGE "message"

#define h_addr h_addr_list[0]

typedef struct s_client_data {
    char    *host;
    int      port;
    SSL     *ssl;
    SSL_CTX *ctx;
    int      server_fd;
    GMutex   data_mutex;
    bool     is_running;  // New flag to signal thread shutdown
    bool     is_connected;
    bool     is_logged_in;
    GList    *messages_list;
    t_user  *current_user;
} t_client_data;

t_users *create_users(t_user *user);
t_user  *create_user(void);
void     add_users_front(t_users **users, t_user *user);
void     free_users(t_users **users);
void     free_user(t_user *user);
void     free_user_data(t_user *user);

t_messages *create_message_list(void);
void free_messages(void);
t_message *create_message_from_response(cJSON *json_message);
void free_message(t_message *message);

cJSON *create_request_registration();
cJSON *create_request_login();
cJSON *create_request_new_private_chat();
cJSON *create_request_all_user_chats(void);
cJSON *create_request_all_users_exclude(void);
cJSON *create_request_new_chat(void);
cJSON *create_get_chat_messages_request(void);
cJSON *create_request_message();

gboolean update_gui_with_response(gpointer data);
int      send_login_request(const gchar *username, const gchar *password, SSL *ssl);
int      send_registration_request(const gchar *username, const gchar *password, SSL *ssl);
int      send_all_user_chats_request(SSL *ssl);
int      send_all_users_exclude_request(t_client_data *client_data);
int      send_create_chat_request(t_app *app, const char *chat_name);
int      send_get_chat_messages_request(int chat_id);
int      send_message_request(gint chat_id, const char *message_text, SSL *ssl);

void handle_login_response(int response_type, cJSON *json_response);
void handle_registration_response(int response_type, cJSON *json_response);
void handle_get_all_user_chats_response(cJSON *response);
void handle_all_users_exclude_response(cJSON *response);
void handle_new_chat_response(cJSON *json_response);
void handle_get_chat_messages_response(cJSON *response);
void handle_new_message_response(cJSON *response);

bool reconnect_to_server(t_client_data *client_data);

t_client_data *create_client_data(const char *host, int port, SSL *ssl, SSL_CTX *ctx, int server_fd);
void           free_client_data(t_client_data *client_data);

extern t_client_data *client_data;
extern GThread       *read_thread;
#endif  // CLIENT_H

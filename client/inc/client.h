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

typedef struct s_client_data {
    char    *host;
    int      port;
    SSL     *ssl;
    SSL_CTX *ctx;
    int      server_fd;
    GMutex   data_mutex;  // Existing mutex for thread safety
    bool     is_running;  // New flag to signal thread shutdown
} t_client_data;

int do_connection(const char *host, int port);

cJSON   *create_request_registration();
cJSON   *create_request_login();
cJSON   *create_request_new_private_chat();
cJSON   *create_request_all_user_chats(void);
gboolean update_gui_with_response(gpointer data);
int      send_login_request(const gchar *username, const gchar *password, SSL *ssl);
int      send_registration_request(const gchar *username, const gchar *password, SSL *ssl);
int      send_all_user_chats_request(SSL *ssl);

void handle_login_response(int response_type);
void handle_registration_response(int response_type);
void handle_get_all_user_chats_response(cJSON *response);

bool reconnect_to_server(t_client_data *client_data, GtkWidget *dialog);

t_client_data *create_client_data(const char *host, int port, SSL *ssl, SSL_CTX *ctx, int server_fd);
void           free_client_data(t_client_data *client_data);
void           shutdown_client(t_client_data *client_data);

extern t_client_data *client_data;
extern GThread       *read_thread;
#endif  // CLIENT_H

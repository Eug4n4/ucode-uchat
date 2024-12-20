#ifndef SERVER_H
#define SERVER_H
#include "common.h"
#include "sqlite3.h"

#define LOGGING_FILE "server.log"
#define HASH_SIZE 64
#define OPENSSL_CERT "cert.pem"
#define OPENSSL_KEY "privatekey.pem"

typedef struct s_chat {
    int   id;
    char *name;
    int   type;

} t_chat;

typedef struct s_chats {
    t_chat         *chat;
    struct s_chats *next;

} t_chats;

typedef struct s_user {
    int   id;
    char *username;
    char *password;
    char *display_name;

} t_user;

typedef struct s_users {
    t_user         *user;
    struct s_users *next;
} t_users;

typedef struct s_accepted_client {
    int  client_fd;
    bool is_logged_in;
    int  client_id;
    SSL *ssl;
} t_accepted_client;

typedef struct s_client_node {
    t_accepted_client    *client;
    struct s_client_node *next;
} t_client_node;

typedef struct s_server_state {
    t_client_node  *client_list_head;
    pthread_mutex_t client_list_mutex;
} t_server_state;

typedef struct s_message {
    int               id;
    char             *sender_username;
    char             *content;
    int64_t           timestamp;
    int               receiver_id;
    struct s_message *next;
} t_message;

typedef struct s_messages {
    t_message *head;
    t_message *tail;
    int        count;
} t_messages;

void daemonize_server(void);

void handle_login_request(cJSON *request, t_accepted_client *client);
void handle_registration_request(cJSON *request, t_accepted_client *client);
void handle_new_private_chat_request(t_server_state *state, cJSON *request, t_accepted_client *client);
void handle_new_group_chat_request(t_server_state *state, cJSON *request, t_accepted_client *client);
void handle_new_chat_request(t_server_state *state, cJSON *request, t_accepted_client *client);
void handle_message_request(cJSON *request, t_accepted_client *client, t_server_state *state);
void handle_all_chats_request(t_accepted_client *client);
void handle_get_chat_messages_request(cJSON *request, t_accepted_client *client);
void handle_get_all_users_exclude_request(cJSON *request, t_accepted_client *client);
void handle_update_message_request(t_server_state *state, cJSON *request);
void handle_delete_message_request(t_server_state *state, cJSON *request);

void generate_login_response(int response, const char *response_message, t_accepted_client *client);
void generate_registration_response(int response, const char *response_message, t_accepted_client *client);
void generate_new_private_chat_response(int response, const char *response_message, t_accepted_client *client);
void generate_new_group_chat_response(int response, const char *response_message, t_accepted_client *client);
void generate_message_response(int response, t_accepted_client *client);
void generate_all_chats_response(int response, t_chats **chats, t_accepted_client *client);
void generate_get_chat_messages_response(int response, t_messages *messages, t_accepted_client *client);
void generate_all_users_exclude_response(int response, t_users **users, t_accepted_client *client);
void generate_update_message_response(int response, t_message *message, t_accepted_client *client);
void generate_delete_message_response(int response_type, t_message *deleted_message, t_accepted_client *client);

void send_response(cJSON *response, t_accepted_client *client);
void send_message_to_online_chat_users(int chat_id, t_accepted_client *sender, const char *message, int64_t timestamp, t_server_state *state);
void process_request_type(cJSON *request, t_accepted_client *client, t_server_state *state);
void process_response_type(int response_type, const char *response_message, t_accepted_client *client);

void notify_new_chat_creation(t_server_state *state, t_accepted_client *client, int chat_id);
void notify_updated_message(t_server_state *state, t_message *message);
void notify_message_deletion(t_server_state *state, t_message *deleted_message);

void    add_client(t_server_state *state, t_accepted_client *client);
void    remove_client(t_server_state *state, t_accepted_client *client);
t_user *create_user(void);
t_user *create_user_with_params(int id, char *username, char *password, char *display_name);
void    free_user(t_user *user);

t_chat  *create_chat(int id, char *name, int type);
void     free_chat(t_chat *chat);
void     add_chat_front(t_chats **chats, t_chat *chat);
t_chats *create_chats(t_chat *chat);
void     free_chats(t_chats **chats);

t_users *create_users(t_user *user);
void     add_users_front(t_users **users, t_user *user);
void     free_users(t_users **users);

t_messages *create_message_list();
void        append_message_to_list(t_messages *messages, t_message *msg);
void        free_messages(t_messages **messages);
void free_message(t_message *message);

void           logging_format(int priority, const char *format, ...);
unsigned char *hash_password(const char *password, int password_len);
char          *hash_to_hex(unsigned char *hash);
#endif

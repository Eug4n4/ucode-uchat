#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/syslog.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "sqlite3.h"
#include "cJSON.h"

#define BUFFER_SIZE 2048

typedef enum e_request_type {
    REGISTRATION,
    LOGIN,
    CREATE_NEW_PRIVATE_CHAT
} t_request_type;

typedef enum e_response_type {
    OK_LOGIN,
    FAIL_LOGIN,
    OK_REGISTRATION,
    FAIL_REGISTRATION,
    OK_CREATE_NEW_PRIVATE_CHAT,
    FAIL_CREATE_NEW_PRIVATE_CHAT
} t_response_type;

typedef struct s_user {
    int id;
    char *username;
    char *password;
    char *display_name;

} t_user;

typedef struct s_accepted_client {
    int client_fd;
    bool is_logged_in;
    int client_id;
} t_accepted_client;

void daemonize_server(void);

void handle_login_request(cJSON *request, t_accepted_client *client);
void handle_registration_request(cJSON *request, t_accepted_client *client);
void handle_new_private_chat_request(cJSON *request, t_accepted_client *client);
void generate_login_response(int response, t_accepted_client *client);
void generate_registration_response(int response, t_accepted_client *client);
void generate_new_private_chat_response(int response, t_accepted_client *client);
void send_response(cJSON *response, t_accepted_client *client);

void process_request_type(cJSON *request, t_accepted_client *client);
void process_response_type(int response_type, t_accepted_client *client);

t_user *create_user(void);
void free_user(t_user *user);

#endif // SERVER_H

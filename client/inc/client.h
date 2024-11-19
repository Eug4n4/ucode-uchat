#ifndef CLIENT_H
#define CLIENT_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "../../libs/cjson/cJSON.h"
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include "../gui/gui_structs.h"

#define KEY_REQUEST_TYPE "request_type"
#define KEY_USERNAME "username"
#define KEY_DISPLAY_NAME "display_name"
#define KEY_PASSWORD "password"
#define KEY_CONTENT "content"

#define KEY_RESPONSE_TYPE "response_type"
#define KEY_MESSAGE "message"

#define h_addr h_addr_list[0]
#define BUF_SIZE 2048

cJSON *create_request_registration();
cJSON *create_request_login();
cJSON *create_request_new_private_chat();
void on_login_clicked(GtkButton *button, gpointer data);

#endif //CLIENT_H


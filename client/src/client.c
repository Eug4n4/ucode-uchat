#define _POSIX_C_SOURCE 1
#include "../inc/client.h"

char *stdin_str_read(char *buf) {
    fgets(buf, BUF_SIZE - 1, stdin);
    buf[strlen(buf) - 1] = '\0';
    return buf;
}

SSL_CTX *setup_ssl_client_context(void) {
    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX *ctx = NULL;

    if (method) {
        ctx = SSL_CTX_new(method);
    }
    if (method == NULL || ctx == NULL) {

        printf("error in setup_ssl_client_context\n");
        return NULL;
    }

    return ctx;
}

void register_user(cJSON *request, char *buf) {
    request = create_request_registration();
    cJSON *request_content = cJSON_GetObjectItemCaseSensitive(request, KEY_CONTENT);

    printf("create username\n>");
    cJSON_ReplaceItemInObjectCaseSensitive(request_content, KEY_USERNAME, cJSON_CreateString(stdin_str_read(buf)));
    cJSON_ReplaceItemInObjectCaseSensitive(request_content, KEY_DISPLAY_NAME, cJSON_CreateString(buf));
    printf("create password\n>");
    cJSON_ReplaceItemInObjectCaseSensitive(request_content, KEY_PASSWORD, cJSON_CreateString(stdin_str_read(buf)));
}

void authorize_user(cJSON *request, char *buf) {
    request = create_request_login();
    cJSON *request_content = cJSON_GetObjectItemCaseSensitive(request, KEY_CONTENT);

    printf("enter username\n>");
    cJSON_ReplaceItemInObjectCaseSensitive(request_content, KEY_USERNAME, cJSON_CreateString(stdin_str_read(buf)));
    printf("enter password\n>");
    cJSON_ReplaceItemInObjectCaseSensitive(request_content, KEY_PASSWORD, cJSON_CreateString(stdin_str_read(buf)));
}

int send_request(cJSON *request, SSL *ssl) {
    char *str = cJSON_PrintUnformatted(request);
    if (str == NULL) {
        return -1;
    }
    int bytes_send = SSL_write(ssl, str, strlen(str));

    if (bytes_send <= 0) {
        int err = SSL_get_error(ssl, bytes_read);

        if (bytes_send == 0) {
            printf("Server disconnected\n");
        } 
        else {
            perror("Error writing to server");
        }
        return -1;
    }
    free(str);
    
    return 0;
}

int get_response(SSL *ssl, char *buf) {
    int bytes_read = SSL_read(ssl, buf, BUF_SIZE);

    if (bytes_read <= 0) {
        int err = SSL_get_error(ssl, bytes_read);

        if (bytes_send == 0) {
            printf("Server disconnected\n");
        } 
        else {
            perror("Error reading from server");
        }
        return -1;
    }

    cJSON *server_response = cJSON_Parse(buf);
    if (server_response == NULL) {
        perror("Failed to parse JSON request\n");
        return -1;
    }

    cJSON *response_type = cJSON_GetObjectItemCaseSensitive(server_response, KEY_RESPONSE_TYPE);
    cJSON *response_content = cJSON_GetObjectItemCaseSensitive(server_response, KEY_CONTENT);
    cJSON *response_message = cJSON_GetObjectItemCaseSensitive(response_content, KEY_MESSAGE);
    int response_type = response_type->valueint;

    printf("%s\n", response_message->valuestring);
    cJSON_Delete(server_response);
    return response_type;
}

int main(int argc, char *argv[]) {
    if (argc < 3) { 
       printf("Usage: %s <host> <port>\n", argv[0]);
       exit(1);
    }
    struct sockaddr_in serv_address;
    struct hostent* server;
    char buf[BUF_SIZE];
    int port = atoi(argv[2]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("Failed to create socket\n");
        exit(1);
    }
    server = gethostbyname(argv[1]);

    if (server == NULL) {
        printf("Host not found\n");
        exit(1);
    }
    memset((char *)&serv_address, 0, sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    memcpy(&serv_address.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_address.sin_port = htons(port);

    if (connect(server_fd, (struct sockaddr*)&serv_address, sizeof(serv_address)) < 0) {
        printf("Connection failed\n");
        exit(1);
    }
    SSL_CTX *ctx = setup_ssl_client_context();
    SSL *ssl = SSL_new(ctx);

    SSL_set_fd(ssl, server_fd);
    if (SSL_connect(ssl) <= 0) {
        printf("Connection failed\n");
        exit(1);
    }
    int ans;
    cJSON *request = NULL;
    int response_type = -1;
    
    while(true) {
        printf("type 0 to register 1 to log in\n>");
        scanf("%d", &ans);
        fgetc(stdin);

        if (ans == 0) {
            register_user(request, buf);
        }
        else if (ans == 1) {
            authorize_user(request, buf);
        }
        if (send_request(request, ssl) == -1) {
            exit(1);
        }     

        response_type = get_response(ssl, buf);
        if (response_type == -1) {
            exit(1);
        }
        if (response_type == OK_LOGIN || OK_REGISTRATION) {
            break;
        }
        cJSON_Delete(request);
        memset(buf, 0, BUF_SIZE);
    }

    cJSON_Delete(request);
    close(server_fd);
    return 0;
}

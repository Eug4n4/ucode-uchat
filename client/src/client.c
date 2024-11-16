#define _POSIX_C_SOURCE 1
#include "client.h"


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

int main(int argc, char * argv[]) {
    if (argc < 3) { 
       printf("Usage: %s <host> <port>\n", argv[0]);
       exit(1);
    }
    struct sockaddr_in serv_address;
    struct hostent* server;
    char buf[BUF_SIZE];
    int port = atoi(argv[2]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0) {
        printf("Failed to create socket\n");
        exit(1);
    }
    server = gethostbyname(argv[1]);
    if(server == NULL) {
        printf("Host not found\n");
        exit(1);
    }
    memset((char *)&serv_address, 0, sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    memcpy(&serv_address.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_address.sin_port = htons(port);

    if(connect(server_fd, (struct sockaddr*)&serv_address, sizeof(serv_address)) < 0) {
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
    cJSON *request;
    cJSON *content;
    cJSON *server_response;
    //while(true) {
        printf("type 0 to register 1 to log in\n>");
        scanf("%d", &ans);
        fgetc(stdin);

        if(ans == 0) {
            request = create_request_registration();
            content = cJSON_GetObjectItemCaseSensitive(request, KEY_CONTENT);

            printf("create username\n>");
            cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_USERNAME, cJSON_CreateString(stdin_str_read(buf)));
            cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_DISPLAY_NAME, cJSON_CreateString(buf));
            printf("create password\n>");
            cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_PASSWORD, cJSON_CreateString(stdin_str_read(buf)));
        }
        else if (ans == 1) {
            request = create_request_login();
            content = cJSON_GetObjectItemCaseSensitive(request, KEY_CONTENT);

            printf("enter username\n>");
            cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_USERNAME, cJSON_CreateString(stdin_str_read(buf)));
            printf("enter password\n>");
            cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_PASSWORD, cJSON_CreateString(stdin_str_read(buf)));
        }
        char *str = cJSON_PrintUnformatted(request);

        if (SSL_write(ssl, str, strlen(str)) < 0) {
            printf("Error writing to socket\n");
            exit(1);
        }
        if(str != NULL) {
            free(str);
        }
        switch (SSL_read(ssl, buf, BUF_SIZE)) {
            case -1:
                printf("Error reading from socket\n");
            exit(1);
            case 0:
                printf("Connection closed\n");
            exit(1);

        }
        server_response = cJSON_Parse(buf);
        if (server_response == NULL) {
            printf("Error receiving response from server\n");
            exit(1);
        }
        cJSON *response_type = cJSON_GetObjectItemCaseSensitive(server_response, KEY_RESPONSE_TYPE);
        cJSON *response_content = cJSON_GetObjectItemCaseSensitive(server_response, KEY_CONTENT);
        cJSON *response_message = cJSON_GetObjectItemCaseSensitive(response_content, KEY_MESSAGE);

        printf("%s\n", response_message->valuestring);
        // if(response_type->valueint == 0 || response_type->valueint == 2) {
        //     break;
        // }
        cJSON_Delete(server_response);
        cJSON_Delete(request);
        memset(buf, 0, BUF_SIZE);
        if(response_type->valueint == 1 || response_type->valueint == 3) {
            return 0;
        }
    //}

    request = create_request_new_private_chat();
    content = cJSON_GetObjectItemCaseSensitive(request, KEY_CONTENT);

    printf("Enter the username of the person you want to start a chat with\n>");
    cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_USERNAME, cJSON_CreateString(stdin_str_read(buf)));

    str = cJSON_PrintUnformatted(request);

    if (SSL_write(ssl, str, strlen(str)) < 0) {
        printf("Error writing to socket\n");
        exit(1);
    }
    if(str != NULL) {
        free(str);
    }

    switch (SSL_read(ssl, buf, BUF_SIZE)) {
    case -1:
        printf("Error reading from socket\n");
        exit(1);
    case 0:
        printf("Connection closed\n");
        exit(1);

    }
    server_response = cJSON_Parse(buf);
    if (server_response == NULL) {
        printf("Error receiving response from server\n");
        exit(1);
    }

    //response_type = cJSON_GetObjectItemCaseSensitive(server_response, KEY_RESPONSE_TYPE);
    response_content = cJSON_GetObjectItemCaseSensitive(server_response, KEY_CONTENT);
    response_message = cJSON_GetObjectItemCaseSensitive(response_content, KEY_MESSAGE);

    printf("%s\n", response_message->valuestring);

    cJSON_Delete(server_response);
    cJSON_Delete(request);

    SSL_CTX_free(ctx);
    SSL_free(ssl);
    close(server_fd);
    return 0;
}

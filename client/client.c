#define _POSIX_C_SOURCE 1
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
#include "../libs/cjson/cJSON.h"

#define KEY_REQUEST_TYPE "request_type"
#define KEY_EMAIL "email"
#define KEY_DISPLAY_NAME "display_name"
#define KEY_PASSWORD "password"
#define KEY_CONTENT "content"

#define h_addr h_addr_list[0]
#define BUF_SIZE 2048

cJSON *create_request_registration() {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, KEY_REQUEST_TYPE, 0);
    cJSON *content = cJSON_CreateObject();
    cJSON_AddStringToObject(content, KEY_EMAIL, "");
    cJSON_AddStringToObject(content, KEY_DISPLAY_NAME, "");
    cJSON_AddStringToObject(content, KEY_PASSWORD, "");
    cJSON_AddItemToObject(json, KEY_CONTENT, content);
    return json;
}

cJSON *create_request_login() {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, KEY_REQUEST_TYPE, 1);
    cJSON *content = cJSON_CreateObject();
    cJSON_AddStringToObject(content, KEY_EMAIL, "");
    cJSON_AddStringToObject(content, KEY_PASSWORD, "");
    cJSON_AddItemToObject(json, KEY_CONTENT, content);
    return json;
}

char *stdin_read(char *buf) {
    fgets(buf, BUF_SIZE - 1, stdin);
    buf[strlen(buf) - 1] = '\0';
    return buf;
}

int main(int argc, char * argv[]) {
    if (argc < 3) { 
       printf("Send host and port as parameters\n");
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
    printf("Looking for the host\n");
    server = gethostbyname(argv[1]);
    if(server == NULL) {
        printf("Host not found\n");
        exit(1);
    }
    memset((char *)&serv_address, 0, sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    memcpy(&serv_address.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_address.sin_port = htons(port);

    printf ("Connecting to server\n");
    if(connect(server_fd, (struct sockaddr*)&serv_address, sizeof(serv_address)) < 0) {
        printf("Connection failed\n");
        exit(1);
    }

    int ans;
    cJSON *request;
    cJSON *content;
    while(true) {
        printf("type 0 to register 1 to log in\n>");
        scanf("%d", &ans);
        fgetc(stdin);

        if(ans == 0) {
            request = create_request_registration();
            content = cJSON_GetObjectItemCaseSensitive(request, KEY_CONTENT);

            printf("enter email\n>");
            cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_EMAIL, cJSON_CreateString(stdin_read(buf)));
            printf("create name\n>");
            cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_DISPLAY_NAME, cJSON_CreateString(stdin_read(buf)));
            printf("create password\n>");
            cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_PASSWORD, cJSON_CreateString(stdin_read(buf)));

        }
        else if (ans == 1) {
            request = create_request_login();
            content = cJSON_GetObjectItemCaseSensitive(request, KEY_CONTENT);

            printf("enter email\n>");
            cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_EMAIL, cJSON_CreateString(stdin_read(buf)));
            printf("enter password\n>");
            cJSON_ReplaceItemInObjectCaseSensitive(content, KEY_PASSWORD, cJSON_CreateString(stdin_read(buf)));

        }
        char *str = cJSON_PrintUnformatted(request);
        if(send(server_fd, str, strlen(str), 0) == -1) {
            printf("%s\n", strerror(errno));
            exit(-1);
        }
        if(str != NULL) {
            free(str);
        }
        break; // temporarily. Waiting for the server update

        int server_response;
        fscanf(fdopen(server_fd, "r+"), "%d", &server_response);
        if(server_response == 0) {
            if(ans == 0) {
                printf("You have successfully registered\n");
            }
            else if (ans == 1) {
                printf("You have successfully logged in\n");
            }
            break;
        }
        else if (server_response == -1){
            printf("Something went wrong\n");
        }
    }
    close(server_fd);
    return 0;
}

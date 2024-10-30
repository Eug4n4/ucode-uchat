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

#define h_addr h_addr_list[0]
#define BUF_SIZE 2048

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
    printf("Write message: ");
    memset(buf, 0, BUF_SIZE);
    fgets(buf, BUF_SIZE - 1, stdin);
    write(server_fd, buf, strlen(buf));
    close(server_fd);
    return 0;
}

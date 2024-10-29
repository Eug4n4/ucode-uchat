#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct sockaddr_in *create_address(int port) {
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));

    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    address->sin_addr.s_addr = INADDR_ANY;
    return address;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Send port as a parameter (8000)\n");
        exit(1);
    }
    int port = atoi(argv[1]);
    char buffer[2048];
    struct sockaddr_in *server_address = create_address(port);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(server_fd, (struct sockaddr *)server_address, sizeof(*server_address)) == -1) {
        free(server_address);
        printf("Error binding\n");
        exit(1);
    }
    if (listen(server_fd, 2) == -1) {
        printf("Error listening to socket\n");
        free(server_address);
        exit(1);
    }
    printf("Server is listening on port %d\n", port);
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        printf("Error accepting connection\n");
        free(server_address);
        exit(1);
    }
    ssize_t bytes_received = read(client_fd, buffer, 2048);
    buffer[bytes_received] = '\0';
    printf("Server received: %s\n", buffer);
    free(server_address);
    close(server_fd);
    return 0;
}

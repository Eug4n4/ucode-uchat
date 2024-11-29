#include "client.h"


int do_connection(const char *host, int port) {
    struct sockaddr_in serv_address;
    struct addrinfo hints, *res;
    int server_fd;

    // Initialize the hints structure for getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP socket

    // Try to resolve the hostname
    int err = getaddrinfo(host, NULL, &hints, &res);
    if (err != 0) {
        printf("getaddrinfo failed: %s\n", gai_strerror(err));
        return -1;
    }

    // Create the socket
    server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_fd < 0) {
        printf("Failed to create socket\n");
        freeaddrinfo(res);
        return -1;
    }

    // Fill in the port number for the sockaddr_in struct
    memset(&serv_address, 0, sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(port);
    memcpy(&serv_address.sin_addr, &((struct sockaddr_in *)res->ai_addr)->sin_addr, sizeof(struct in_addr));

    // Attempt to connect
    if (connect(server_fd, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0) {
        printf("Connection failed\n");
        close(server_fd);
        freeaddrinfo(res);
        return -1;
    }

    freeaddrinfo(res);
    return server_fd;
}


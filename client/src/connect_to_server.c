#include "client.h"

int do_connection(const char *host, int port) {
    struct sockaddr_in serv_address;
    struct addrinfo    hints, *res;
    int                server_fd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(host, NULL, &hints, &res);
    if (err != 0) {
        return -1;
    }

    server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_fd < 0) {
        freeaddrinfo(res);
        return -1;
    }

    memset(&serv_address, 0, sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    serv_address.sin_port   = htons(port);
    memcpy(&serv_address.sin_addr, &((struct sockaddr_in *)res->ai_addr)->sin_addr, sizeof(struct in_addr));

    if (connect(server_fd, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0) {
        close(server_fd);
        freeaddrinfo(res);
        return -1;
    }

    freeaddrinfo(res);
    return server_fd;
}

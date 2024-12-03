#include "common.h"

static int setup_client(int socket_fd, const char *host, int port) {
    struct sockaddr_in address;
    struct hostent    *server;
    
    if (socket_fd < 0) {
        return -1;
    }
    server = gethostbyname(host);
    if (server == NULL) {
        return -1;
    }
    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    memcpy(&address.sin_addr.s_addr, server->h_addr, server->h_length);
    address.sin_port = htons(port);

    if (connect(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        return -1;
    }
    return socket_fd;
}


static int setup_server(int socket_fd, int port) {
    struct sockaddr_in serv_address;
   
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(port);
    serv_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(socket_fd, (struct sockaddr *)&serv_address, sizeof(serv_address)) == -1) {
        return -1;
    }
    if (listen(socket_fd, 2) == -1) {
        return -1;
    }
    return socket_fd;
}

int do_connection(const char *host, int port) {
    int                server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int sock = -1;

    if (host) {
        sock = setup_client(server_fd, host, port);
    } else {
        sock = setup_server(server_fd, port);
    }
    return sock;
}



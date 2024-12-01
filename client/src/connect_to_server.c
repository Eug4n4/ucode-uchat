#include "client.h"

int do_connection(const char *host, int port) {
    struct sockaddr_in serv_address;
    struct hostent    *server;
    int                server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        return -1;
    }
    server = gethostbyname(host);
    if (server == NULL) {
        return -1;
    }
    memset((char *)&serv_address, 0, sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    memcpy(&serv_address.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_address.sin_port = htons(port);

    if (connect(server_fd, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0) {
        return -1;
    }
    return server_fd;
}

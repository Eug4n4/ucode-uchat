#define _POSIX_C_SOURCE 1
#include "../inc/client.h"
#include "../inc/gui.h"
#include <gtk/gtk.h>

SSL_CTX *setup_ssl_client_context(void) {
    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX          *ctx    = NULL;

    if (method) {
        ctx = SSL_CTX_new(method);
    }
    if (!ctx) {
        printf("Error in setup_ssl_client_context\n");
        return NULL;
    }
    return ctx;
}

gpointer read_from_server_thread(gpointer data) {
    SSL *ssl = (SSL *)data;
    char buffer[BUF_SIZE] = {0};

    while (1) {
        int bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("Connection closed by the server.\n");
            } else {
                printf("SSL read error.\n");
            }
            break;
        }
        // buffer[bytes_read] = '\0';

        g_idle_add((GSourceFunc)update_gui_with_response, g_strdup(buffer));
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <host> <port>\n", argv[0]);
        exit(1);
    }

    struct sockaddr_in serv_address;
    struct hostent    *server;
    int                port      = atoi(argv[2]);
    int                server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("Failed to create socket\n");
        exit(1);
    }

    server = gethostbyname(argv[1]);
    if (!server) {
        printf("Host not found\n");
        exit(1);
    }

    memset(&serv_address, 0, sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    memcpy(&serv_address.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_address.sin_port = htons(port);

    if (connect(server_fd, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0) {
        printf("Connection failed\n");
        exit(1);
    }

    SSL_CTX *ctx = setup_ssl_client_context();
    SSL     *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, server_fd);
    if (SSL_connect(ssl) <= 0) {
        printf("SSL connection failed\n");
        exit(1);
    }

    init_gui(argc, argv, ssl);

    g_thread_new("read-from-server", read_from_server_thread, ssl);

    gtk_main();

    SSL_CTX_free(ctx);
    SSL_free(ssl);
    close(server_fd);

    return 0;
}

#define _POSIX_C_SOURCE 1
#include "../inc/client.h"
#include "../inc/gui.h"

gpointer read_from_server_thread(gpointer data) {
    SSL *ssl = (SSL *)data;
    char buffer[BUF_SIZE] = {0};

    while (true) {
        int bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);

        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("Connection closed by the server.\n");
            } else {
                int res = SSL_get_error(ssl, bytes_read);

                if (res == SSL_ERROR_WANT_READ || res == SSL_ERROR_WANT_WRITE) {
                    continue;
                }
                printf("SSL read error %d\n", res);
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
        exit(EXIT_FAILURE);
    }
    int server_fd = do_connection(argv[1], atoi(argv[2]));
    SSL_CTX *ctx = setup_ssl_context(false);
    SSL     *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, server_fd);
    
    if (SSL_connect(ssl) <= 0) {
        printf("SSL connection failed\n");
        exit(EXIT_FAILURE);
    }

    init_gui(argc, argv, ssl);

    g_thread_new("read-from-server", read_from_server_thread, ssl);

    gtk_main();

    SSL_CTX_free(ctx);
    SSL_free(ssl);
    close(server_fd);

    return 0;
}


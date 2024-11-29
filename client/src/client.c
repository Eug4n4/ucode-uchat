#define _POSIX_C_SOURCE 1
#include "../inc/client.h"

gpointer read_from_server_thread(gpointer data) {
    t_connection *connection = (t_connection *)data;
    SSL *ssl = connection->ssl;
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
        connection->buffer = mx_strdup(buffer);
        g_idle_add((GSourceFunc)update_gui_with_response, connection);
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
    t_app *app = malloc(sizeof(t_app));
    t_connection *connection = malloc(sizeof(t_connection));
    connection->server_fd = server_fd;
    connection->ssl = ssl;
    // connection->user = NULL; // do we really need to store user data on client side? Maybee for creating new chat request (yes)
    app->connection = connection;
    app->current_user = NULL;
    app->users = NULL;
    init_gui(argc, argv, app);

    g_thread_new("read-from-server", read_from_server_thread, connection);

    gtk_main();

    SSL_CTX_free(ctx);
    SSL_free(ssl);
    free(app->connection);
    free(app);
    close(server_fd);

    return 0;
}


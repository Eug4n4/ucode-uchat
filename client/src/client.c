#define _POSIX_C_SOURCE 1
#include "../inc/client.h"

t_client_data *client_data = NULL;

// client.c
bool reconnect_to_server(t_client_data *client_data) {
    printf("Attempting to reconnect...\n");

    int new_server_fd = do_connection(client_data->host, client_data->port);
    if (new_server_fd < 0) {
        printf("Host not found. Retrying...\n");
        return false;
    }

    // Create a new SSL object and connect
    SSL *new_ssl = SSL_new(client_data->ctx);
    SSL_set_fd(new_ssl, new_server_fd);

    if (SSL_connect(new_ssl) <= 0) {
        printf("SSL connection failed. Retrying...\n");
        close(new_server_fd);  // Close the socket if SSL connection fails
        SSL_free(new_ssl);     // Free the SSL object if connection fails
        return false;
    }

    // Lock mutex and update the client data with new SSL and server_fd
    g_mutex_lock(&client_data->data_mutex);
    SSL_free(client_data->ssl);              // Free old SSL object
    client_data->ssl       = new_ssl;        // Update with the new SSL object
    client_data->server_fd = new_server_fd;  // Update the server_fd
    g_mutex_unlock(&client_data->data_mutex);

    return true;
}

// client.c
gpointer read_from_server_thread(gpointer data) {
    t_client_data *client_data      = (t_client_data *)data;
    char           buffer[BUF_SIZE] = { 0 };
    bool           reconnecting     = false;

    while (true) {
        g_mutex_lock(&client_data->data_mutex);
        SSL *ssl       = client_data->ssl;        // Access SSL inside mutex
        int  server_fd = client_data->server_fd;  // Get current server_fd
        g_mutex_unlock(&client_data->data_mutex);

        int bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            int res = SSL_get_error(ssl, bytes_read);

            if (res != SSL_ERROR_WANT_READ && res != SSL_ERROR_WANT_WRITE) {
                // g_mutex_lock(&client_data->data_mutex);
                // if (client_data->ssl != NULL) {
                //     SSL_shutdown(client_data->ssl);  // Gracefully shut down SSL
                //     SSL_free(client_data->ssl);      // Free the SSL object
                //     client_data->ssl = NULL;
                // }
                // if (client_data->server_fd >= 0) {
                //     close(client_data->server_fd);  // Close the socket
                //     client_data->server_fd = -1;    // Reset server_fd
                // }
                // g_mutex_unlock(&client_data->data_mutex);
                if (!reconnecting) {
                    reconnecting = true;

                    GtkWidget *popup = show_reconnect_popup(client_data->host, client_data->port, &client_data->ssl, client_data->ctx);
                    while (!reconnect_to_server(client_data)) {
                        printf("Reconnect failed, retrying...\n");
                        g_usleep(1000000);  // Wait before retrying
                    }

                    printf("Reconnected successfully!\n");
                    g_idle_add((GSourceFunc)close_reconnect_popup, popup);  // Close popup when idle
                    reconnecting = false;
                }
            }
            continue;
        }

        buffer[bytes_read] = '\0';                                            // Null-terminate the buffer
        g_idle_add((GSourceFunc)update_gui_with_response, g_strdup(buffer));  // Update GUI with server response
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <host> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    SSL_CTX    *ctx       = setup_ssl_context(false);
    const char *host      = argv[1];
    int         port      = atoi(argv[2]);
    int         server_fd = do_connection(host, port);

    if (server_fd < 0) {
        printf("Connection failed\n");
        exit(EXIT_FAILURE);
    }

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, server_fd);

    if (SSL_connect(ssl) <= 0) {
        printf("SSL connection failed\n");
        exit(EXIT_FAILURE);
    }

    client_data = create_client_data(host, port, ssl, ctx, server_fd);
    printf("client_data in main: %p\n", (void *)client_data);
    init_gui(argc, argv, ssl);

    g_thread_new("read-from-server", read_from_server_thread, client_data);
    gtk_main();

    free_client_data(client_data);
    SSL_CTX_free(ctx);

    return 0;
}

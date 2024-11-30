#define _POSIX_C_SOURCE 1
#include "../inc/client.h"

GThread       *read_thread = NULL;
t_client_data *client_data = NULL;

bool reconnect_to_server(t_client_data *client_data, GtkWidget *dialog) {
    if (client_data->is_running) {
        printf("Attempting to reconnect...\n");

        int new_server_fd = do_connection(client_data->host, client_data->port);
        if (new_server_fd < 0) {
            return false;
        }

        SSL *new_ssl = SSL_new(client_data->ctx);
        SSL_set_fd(new_ssl, new_server_fd);

        if (SSL_connect(new_ssl) <= 0) {
            close(new_server_fd);
            SSL_free(new_ssl);
            return false;
        }

        g_mutex_lock(&client_data->data_mutex);
        client_data->ssl       = new_ssl;
        client_data->server_fd = new_server_fd;
        g_mutex_unlock(&client_data->data_mutex);

        return true;
    }
    return false;
}

// client.c
gpointer read_from_server_thread(gpointer data) {
    client_data            = (t_client_data *)data;
    char buffer[BUF_SIZE]  = { 0 };
    bool reconnecting      = false;
    int  reconnect_attempt = 0;

    while (client_data->is_running) {
        g_mutex_lock(&client_data->data_mutex);
        SSL *ssl = client_data->ssl;
        g_mutex_unlock(&client_data->data_mutex);

        if (!ssl) {
            printf("SSL object is null, terminating thread.\n");
            break;
        }

        int bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            int res = SSL_get_error(ssl, bytes_read);

            if (res != SSL_ERROR_WANT_READ && res != SSL_ERROR_WANT_WRITE) {
                if (!reconnecting) {
                    reconnecting = true;

                    SSL_shutdown(ssl);
                    close(client_data->server_fd);
                    SSL_free(ssl);

                    GtkWidget *popup = show_reconnect_popup(client_data->host, client_data->port, &client_data->ssl, client_data->ctx);

                    while (!reconnect_to_server(client_data, popup)) {
                        if (!client_data->is_running) {
                            return NULL;
                        }

                        reconnect_attempt++;
                        int delay = (int)pow(2, reconnect_attempt);  // Exponential backoff

                        // Update the dialog with the current attempt
                        char message[256];
                        snprintf(message, sizeof(message), "Reconnection attempt #%d failed. Retrying in %d seconds...", reconnect_attempt, delay);
                        gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(popup), message);

                        g_usleep(delay * 1000000);  // Sleep for `delay` seconds
                    }

                    printf("Reconnected successfully!\n");
                    reconnect_attempt = 0;  // Reset counter on success
                    g_idle_add((GSourceFunc)close_reconnect_popup, popup);
                    reconnecting = false;
                }
            }
        }

        buffer[bytes_read] = '\0';
        g_idle_add((GSourceFunc)update_gui_with_response, g_strdup(buffer));
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (!XInitThreads()) {
        printf("Failed to initialize X11 threads\n");
        exit(EXIT_FAILURE);
    }

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
    init_gui(argc, argv, ssl);

    read_thread = g_thread_new("read-from-server", read_from_server_thread, client_data);
    gtk_main();

    free_client_data(client_data);
    SSL_CTX_free(ctx);

    return 0;
}

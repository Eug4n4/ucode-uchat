#include "../inc/client.h"

GThread       *read_thread = NULL;
t_client_data *client_data = NULL;

bool reconnect_to_server(t_client_data *client_data) {
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


gpointer read_from_server_thread(gpointer data) {
    char buffer[BUF_SIZE]  = { 0 };
    bool reconnecting      = false;
    int  reconnect_attempt = 0;

    while (client_data->is_running) {

        if (!client_data->ssl) {
            printf("SSL object is null, terminating thread.\n");
            break;
        }
        if (client_data->is_running && client_data->is_connected) {
            int bytes_read = SSL_read(client_data->ssl, buffer, sizeof(buffer) - 1);
            int res = 0;
            if (bytes_read <= 0) {
                res = SSL_get_error(client_data->ssl, bytes_read);

                if (res != SSL_ERROR_WANT_READ && res != SSL_ERROR_WANT_WRITE && client_data->is_running == true) {
                    if (!reconnecting) {
                        reconnecting = true;

                        SSL_shutdown(client_data->ssl);
                        close(client_data->server_fd);
                        SSL_free(client_data->ssl);

                        GtkWidget *popup = show_reconnect_popup();

                        while (!reconnect_to_server(client_data)) {
                            if (!client_data->is_running) {
                                return NULL;
                            }

                            g_mutex_lock(&client_data->data_mutex);
                            client_data->is_connected = false;
                            g_mutex_unlock(&client_data->data_mutex);
                            reconnect_attempt++;
                            int delay = (int)pow(2, reconnect_attempt);

                            // Update the dialog with the current attempt
                            char message[256];
                            snprintf(message, sizeof(message), "Reconnection attempt #%d failed. Retrying in %d seconds...", reconnect_attempt, delay);
                            gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(popup), message);

                            g_usleep(delay * 1000000);
                        }

                        printf("Reconnected successfully!\n");
                        g_mutex_lock(&client_data->data_mutex);
                        client_data->is_connected = true;
                        g_mutex_unlock(&client_data->data_mutex);
                        if (client_data->is_logged_in) {
                            if (send_login_request(client_data->current_user->username, client_data->current_user->password, client_data->ssl) < 0) {
                                gtk_label_set_text(gtk_sign_in->label_error, "Error communicating with server");
                            }
                        }
                        reconnect_attempt = 0;
                        g_idle_add((GSourceFunc)close_reconnect_popup, popup);
                        reconnecting = false;
                    }
                }
            } else if ((res == SSL_ERROR_WANT_READ || res == SSL_ERROR_WANT_WRITE) && client_data->is_running) {
                continue;
            }

            buffer[bytes_read] = '\0';
            g_idle_add((GSourceFunc)update_gui_with_response, g_strdup(buffer));
        }
    }
    (void)data;
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
    t_app *app = malloc(sizeof(t_app));
    app->current_user = NULL;
    app->users        = NULL;

    client_data = create_client_data(host, port, ssl, ctx, server_fd);
    g_mutex_lock(&client_data->data_mutex);
    client_data->is_connected = true;
    g_mutex_unlock(&client_data->data_mutex);
    init_gui(argc, argv, app);

    read_thread = g_thread_new("read-from-server", read_from_server_thread, NULL);
    gtk_main();

    SSL_CTX_free(ctx);
    free(app);
    return 0;
}

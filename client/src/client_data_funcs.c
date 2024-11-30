#include "client.h"

t_client_data *create_client_data(const char *host, int port, SSL *ssl, SSL_CTX *ctx, int server_fd) {
    t_client_data *data = malloc(sizeof(t_client_data));
    if (!data)
        return NULL;

    data->host      = strdup(host);
    data->port      = port;
    data->ssl       = ssl;
    data->ctx       = ctx;
    data->server_fd = server_fd;
    g_mutex_init(&data->data_mutex);
    data->is_running = true;
    return data;
}

void free_client_data(t_client_data *client_data) {
    if (!client_data)
        return;

    g_mutex_lock(&client_data->data_mutex);
    client_data->is_running = false;

    if (client_data->ssl) {
        SSL_shutdown(client_data->ssl);
        SSL_free(client_data->ssl);
    }
    if (client_data->server_fd > 0) {
        close(client_data->server_fd);
    }
    client_data->ssl       = NULL;
    client_data->server_fd = -1;
    g_mutex_unlock(&client_data->data_mutex);

    g_mutex_clear(&client_data->data_mutex);
    free(client_data);
}

void shutdown_client(t_client_data *client_data) {
    g_mutex_lock(&client_data->data_mutex);
    client_data->is_running = false;
    g_mutex_unlock(&client_data->data_mutex);

    if (client_data->ssl) {
        SSL_shutdown(client_data->ssl);
        SSL_free(client_data->ssl);
    }
    if (client_data->server_fd >= 0) {
        close(client_data->server_fd);
    }

    g_thread_join(read_thread);
}

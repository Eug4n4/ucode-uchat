#include "../inc/client.h"

t_client_data *create_client_data(const char *host, int port, SSL *ssl, SSL_CTX *ctx, int server_fd) {
    t_client_data *data = malloc(sizeof(t_client_data));
    if (!data)
        return NULL;

    data->host      = (char*)host;
    data->port      = port;
    data->ssl       = ssl;
    data->ctx       = ctx;
    data->server_fd = server_fd;
    g_mutex_init(&data->data_mutex);
    data->is_running   = true;
    data->is_connected = false;
    data->is_logged_in = false;
    data->messages_list = NULL;
    data->id_button_table = g_hash_table_new_full(g_direct_hash, g_direct_equal, g_free, NULL);
    data->current_user = create_user();
    return data;
}

void free_client_data(t_client_data *client_data) {
    if (!client_data)
        return;

    g_mutex_lock(&client_data->data_mutex);
    client_data->is_running = false;

    if (client_data->ssl) {
        SSL_shutdown(client_data->ssl);
        if (client_data->is_connected) {
            SSL_free(client_data->ssl);
        }
    }
    if (client_data->server_fd > 0) {
        close(client_data->server_fd);
    }
    client_data->ssl       = NULL;
    client_data->server_fd = -1;

    free_user(client_data->current_user);
    g_mutex_unlock(&client_data->data_mutex);
    g_mutex_clear(&client_data->data_mutex);
    free(client_data);
    client_data = NULL;
}



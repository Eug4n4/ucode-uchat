#include "client.h"

t_client_data *create_client_data(const char *host, int port, SSL *ssl, SSL_CTX *ctx, int server_fd) {
    t_client_data *client_data = malloc(sizeof(t_client_data));
    client_data->host          = strdup(host);  // Allocate and copy host
    client_data->port          = port;
    client_data->ssl           = ssl;
    client_data->ctx           = ctx;
    client_data->server_fd     = server_fd;  // Initialize server_fd
    g_mutex_init(&client_data->data_mutex);  // Initialize mutex
    return client_data;
}

void free_client_data(t_client_data *client_data) {
    if (client_data) {
        free(client_data->host);
        g_mutex_clear(&client_data->data_mutex);  // Clear mutex
        if (client_data->ssl) {
            SSL_shutdown(client_data->ssl);
            SSL_free(client_data->ssl);
        }
        if (client_data->server_fd >= 0) {
            close(client_data->server_fd);  // Close the socket
        }
        free(client_data);
    }
}

#include "server.h"


void *serve_client(void *args) {
    intptr_t *arg = (intptr_t *)args;
    int client_fd = (int)arg[0];                      
    t_server_state *state = (t_server_state *)arg[1];
    SSL_CTX *ctx = (SSL_CTX *)arg[2];
    SSL *ssl = SSL_new(ctx);

    char buffer[BUF_SIZE] = { 0 };
    t_accepted_client *client = malloc(sizeof(t_accepted_client));
    client->client_fd = client_fd;
    client->is_logged_in = false;
    client->client_id = -1;
    client->ssl = ssl;
    if (!SSL_set_fd(client->ssl, client->client_fd)) {
        logging_format(LOG_ERR, "SSL_set_fd(client->ssl, client->client_fd) failed\n");
        free(client);
        SSL_CTX_free(ctx);
        SSL_free(ssl);
        exit(EXIT_FAILURE);
    }
    int ret = SSL_accept(client->ssl);
    if (ret != 1) {
        logging_format(LOG_ERR, "SSL_accept() failed %d\n", ret);
        free(client);
        SSL_CTX_free(ctx);
        SSL_free(ssl);
        exit(EXIT_FAILURE);
    }
    add_client(state, client);

    while (true) {
        int bytes_read = SSL_read(client->ssl, buffer, sizeof(buffer));

        if (bytes_read <= 0) {
            int err = SSL_get_error(client->ssl, bytes_read);

            if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
                continue;
            }
            break;
        }


        cJSON *request = cJSON_Parse(buffer);

        if (request == NULL) {
            logging_format(LOG_ERR, "Failed to parse JSON request\n");
            continue;
        }

        process_request_type(request, client, state);
        cJSON_Delete(request);
        memset(buffer, 0, sizeof(buffer));
    }
    remove_client(state, client);
    close(client_fd);
    return NULL;
}


void load_cert_and_key(SSL_CTX *ctx) {
    if (SSL_CTX_use_certificate_file(ctx, OPENSSL_CERT, SSL_FILETYPE_PEM) <= 0) {
        logging_format(LOG_ERR,"Error in load_cert_and_key\n");
        return;
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, OPENSSL_KEY, SSL_FILETYPE_PEM) <= 0) {
        logging_format(LOG_ERR,"Error in load_cert_and_key\n");
        return;
    }

    if (SSL_CTX_check_private_key(ctx) != 1) {
        logging_format(LOG_ERR,"Error in load_cert_and_key\n");
    }
}


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // int debug_mode = atoi(argv[2]);
    daemonize_server();
    // if (!debug_mode) {
    //     daemonize_server();
    // }
    SSL_CTX *ctx = setup_ssl_context(true);
    load_cert_and_key(ctx);
    
    int port = atoi(argv[1]);
    int server_fd = do_connection(NULL, port);


    t_server_state state = { .client_list_head = NULL, .client_list_mutex = PTHREAD_MUTEX_INITIALIZER };

    while (true) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd == -1) {
            SSL_CTX_free(ctx);
            exit(EXIT_FAILURE);
        }

        pthread_t thread;
        intptr_t *args = malloc(3 * sizeof(intptr_t));  
        args[0] = (intptr_t)client_fd;                  
        args[1] = (intptr_t)&state;
        args[2] = (intptr_t)ctx;
        if (pthread_create(&thread, NULL, serve_client, args) != 0) {
            free(args);
            break;
        }
        pthread_detach(thread);
    }

    close(server_fd);
    return 0;
}


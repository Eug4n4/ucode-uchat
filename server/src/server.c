#include "server.h"
#include "db.h"

struct sockaddr_in *create_address(int port) {
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    address->sin_addr.s_addr = INADDR_ANY;
    return address;
}

void *serve_client(void *args) {
    intptr_t *arg = (intptr_t *)args;
    int client_fd = (int)arg[0];                       // Retrieve client_fd
    t_server_state *state = (t_server_state *)arg[1];  // Retrieve pointer to t_server_state
    SSL_CTX *ctx = (SSL_CTX *)arg[2];
    SSL *ssl = SSL_new(ctx);

    char buffer[BUF_SIZE] = { 0 };
    t_accepted_client *client = malloc(sizeof(t_accepted_client));
    client->client_fd = client_fd;
    client->is_logged_in = false;
    client->client_id = -1;
    client->ssl = ssl;
    if (!SSL_set_fd(client->ssl, client->client_fd)) {
        printf("SSL_set_fd(client->ssl, client->client_fd) failed\n");
        free(client);
        SSL_CTX_free(ctx);
        SSL_free(ssl);
        exit(EXIT_FAILURE);
    }
    int ret = SSL_accept(client->ssl);
    if (ret != 1) {
        printf("SSL_accept() failed %d\n", ret);
        free(client);
        SSL_CTX_free(ctx);
        SSL_free(ssl);
        exit(EXIT_FAILURE);
    }
    add_client(state, client);

    while (true) {
        // ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
        int bytes_read = SSL_read(client->ssl, buffer, sizeof(buffer));

        if (bytes_read <= 0) {
            int err = SSL_get_error(client->ssl, bytes_read);

            if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
                continue;
            }
            if (bytes_read == 0) {
                printf("Client disconnected.\n");
            } else {
                perror("Error reading from client");
            }
            break;
        }

        printf("Received request: %s\n", buffer);
        cJSON *request = cJSON_Parse(buffer);

        if (request == NULL) {
            printf("Failed to parse JSON request\n");
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
        printf("Error in load_cert_and_key\n");
        return;
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, OPENSSL_KEY, SSL_FILETYPE_PEM) <= 0) {
        printf("Error in load_cert_and_key\n");
        return;
    }

    if (SSL_CTX_check_private_key(ctx) != 1) {
        printf("Error in load_cert_and_key\n");
    }
}


int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <port> <debug>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
   
    int debug_mode = atoi(argv[2]);
    if (!debug_mode) {
        daemonize_server();
    }
    SSL_CTX *ctx = setup_ssl_context(true);
    load_cert_and_key(ctx);
    
    int port = atoi(argv[1]);
    struct sockaddr_in *server_address = create_address(port);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(server_fd, (struct sockaddr *)server_address, sizeof(*server_address)) == -1) {
        free(server_address);
        if (debug_mode) {
            perror("Error binding");
        } else {
            logging_format(LOG_ERR, "Error binding");
        }
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 2) == -1) {
        free(server_address);
        if (debug_mode) {
            perror("Error listening");
        } else {
            logging_format(LOG_ERR, "Error listening");
        }
        exit(EXIT_FAILURE);
    }

    if (debug_mode) {
        printf("Server started with PID: %d\n", getpid());
    } else {
        logging_format(LOG_INFO, "Server started with PID: %d", getpid());
    }

    t_server_state state = { .client_list_head = NULL, .client_list_mutex = PTHREAD_MUTEX_INITIALIZER };

    while (true) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd == -1) {
            free(server_address);
            if (debug_mode) {
                perror("Error accepting connection");
            }
            exit(EXIT_FAILURE);
        }

        pthread_t thread;
        intptr_t *args = malloc(3 * sizeof(intptr_t));  // Allocate space for two intptr_t values
        args[0] = (intptr_t)client_fd;                  // Store client_fd as intptr_t (it will hold integer values safely)
        args[1] = (intptr_t)&state;                     // Store the address of state as intptr_t
        args[2] = (intptr_t)ctx;
        if (pthread_create(&thread, NULL, serve_client, args) != 0) {
            free(args);
            break;
        }
        pthread_detach(thread);
    }

    free(server_address);
    close(server_fd);
    return 0;
}

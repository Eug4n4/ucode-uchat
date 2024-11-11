#include "db.h"

struct sockaddr_in *create_address(int port) {
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    address->sin_addr.s_addr = INADDR_ANY;
    return address;
}

void *serve_client(void *fd) {
    char buffer[BUFFER_SIZE];
    int client_fd = *(int *)fd;
    t_accepted_client *client = malloc(sizeof(t_accepted_client));
    client->client_fd = client_fd;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));

        if (bytes_read <= 0) {
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

        process_request_type(request, client);
        cJSON_Delete(request);
    }

    free(client);
    close(client_fd);
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <port> <debug>\n", argv[0]);
        exit(1);
    }

    int debug_mode = atoi(argv[2]);

    if (!debug_mode) {
        daemonize_server();
    }

    int port = atoi(argv[1]);
    struct sockaddr_in *server_address = create_address(port);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(server_fd, (struct sockaddr *)server_address, sizeof(*server_address)) == -1) {
        free(server_address);
        if (debug_mode) {
            perror("Error binding");
        } else {
            syslog(LOG_ERR, "Error binding");
        }
        exit(1);
    }

    if (listen(server_fd, 2) == -1) {
        free(server_address);
        if (debug_mode) {
            perror("Error listening");
        } else {
            syslog(LOG_ERR, "Error listening");
        }
        exit(1);
    }

    if (debug_mode) {
        printf("Server started in debug mode (PID: %d)\n", getpid());
    } else {
        syslog(LOG_INFO, "Server started with PID: %d", getpid());
    }

    while (true) {
        int client_fd = accept(server_fd, NULL, NULL);

        if (client_fd == -1) {
            free(server_address);
            if (debug_mode) {
                perror("Error accepting connection");
            }
            exit(1);
        }

        pthread_t thread;
        if (pthread_create(&thread, NULL, serve_client, &client_fd) != 0) {
            break;
        }

        pthread_detach(thread);
    }

    free(server_address);
    close(server_fd);
    return 0;
}

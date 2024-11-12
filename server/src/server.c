#include "db.h"
#include "server.h"

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

    char buffer[BUFFER_SIZE];
    t_accepted_client *client = malloc(sizeof(t_accepted_client));
    client->client_fd = client_fd;
    client->is_logged_in = false;
    client->client_id = -1;

    add_client(state, client);

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

        process_request_type(request, client, state);
        cJSON_Delete(request);
    }

    remove_client(state, client);
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
        printf("Server started with PID: %d\n", getpid());
    } else {
        syslog(LOG_INFO, "Server started with PID: %d", getpid());
    }

    t_server_state state = { .client_list_head = NULL, .client_list_mutex = PTHREAD_MUTEX_INITIALIZER };
    printf("Initial client_list_head: %p\n", state.client_list_head);
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
        intptr_t *args = malloc(2 * sizeof(intptr_t));  // Allocate space for two intptr_t values
        args[0] = (intptr_t)client_fd;                  // Store client_fd as intptr_t (it will hold integer values safely)
        args[1] = (intptr_t)&state;                     // Store the address of state as intptr_t

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

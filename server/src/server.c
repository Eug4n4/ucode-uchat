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
    read(client_fd, buffer, BUFFER_SIZE);
    cJSON *request = cJSON_Parse(buffer);
    process_request_type(request, client);
    free(client);
    close(client_fd);
    return NULL;
}


int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    daemonize_server();

    int port = atoi(argv[1]);
    struct sockaddr_in *server_address = create_address(port);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(server_fd, (struct sockaddr *)server_address, sizeof(*server_address)) == -1) {
        free(server_address);
        syslog(LOG_ERR, "Error binding");
        exit(1);
    }
    if (listen(server_fd, 2) == -1) {
        free(server_address);
        syslog(LOG_ERR, "Error listening");
        exit(1);
    }

    syslog(LOG_INFO, "Server started with PID: %d", getpid());
    while (true) {
        int client_fd = accept(server_fd, NULL, NULL);

        if (client_fd == -1) {
            free(server_address);
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





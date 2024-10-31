#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/syslog.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct sockaddr_in *create_address(int port) {
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));

    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    address->sin_addr.s_addr = INADDR_ANY;
    return address;
}

void *serve_client(void *fd) {
    char buffer[2048];
    int client_fd = *(int *)fd;
    ssize_t bytes_received = read(client_fd, buffer, 2048);

    buffer[bytes_received] = '\0';
    syslog(LOG_INFO, "Server received: %s", buffer);
    close(client_fd);
    return NULL;
}

void daemonize_server(void) {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Error forking\n");
        exit(1);
    }
    if (pid != 0) {
        exit(0);
    }
    if (setsid() == -1) {
        printf("Error setsid\n");
        exit(1);
    }
    pid = fork();
    if (pid < 0) {
        printf("Error forking\n");
        exit(1);
    }
    if (pid != 0) {
        printf("Daemon started. PID: %d\n", pid);
        exit(0);
    }
    struct rlimit rlim;

    chdir("/");
    getrlimit(RLIMIT_NOFILE, &rlim);
    for (int fd = 0; fd < rlim.rlim_max; ++fd) {
        close(fd);
    }
    openlog(NULL, LOG_PID, LOG_DAEMON);
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





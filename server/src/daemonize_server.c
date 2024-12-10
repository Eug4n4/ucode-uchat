#include "server.h"

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

    
    getrlimit(RLIMIT_NOFILE, &rlim);
    for (size_t fd = 0; fd < rlim.rlim_max; ++fd) {
        close(fd);
    }

}


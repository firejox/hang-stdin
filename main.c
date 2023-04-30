#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if defined(__linux__) && defined(USE_EVENT_FD)
#include <sys/eventfd.h>
#elif defined(USE_UNIX_SOCKET)
#include <sys/socket.h>
#include <sys/un.h>
#endif

int main(int argc, const char * const argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Usage: %s [command [argument ...]]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

#if defined(__linux__) && defined(USE_EVENT_FD)
    int fd;

    if ((fd = eventfd(0, 0)) == -1) {
        perror("create eventfd failed");
        exit(EXIT_FAILURE);
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("replace stdin fileno failed");
        exit(EXIT_FAILURE);
    }

    close(fd);
#elif defined(USE_UNIX_SOCKET)
    int fd;

    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("create unix socket failed");
        exit(EXIT_FAILURE);
    }

    if (shutdown(fd, SHUT_WR) == -1) {
        perror("make socket read-only failed");
        exit(EXIT_FAILURE);
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("replace stdin fileno failed");
        exit(EXIT_FAILURE);
    }

    close(fd);
#else
    int pfd[2];

    if (pipe(pfd) == -1) {
        perror("create pipe failed");
        exit(EXIT_FAILURE);
    }

    if (dup2(pfd[0], STDIN_FILENO) == -1) {
        perror("replace stdin fileno failed");
        exit(EXIT_FAILURE);
    }

    close(pfd[0]);
#endif

    char *new_env[] = { NULL };
    const char *program = argv[1];
    char *new_argv[argc];

    memcpy(new_argv, argv + 1, sizeof(new_argv) - sizeof(char*));
    new_argv[argc - 1] = NULL;

    execve(program, new_argv, new_env);

    perror("spawn process failure");
    exit(EXIT_FAILURE);
    return 0;
}

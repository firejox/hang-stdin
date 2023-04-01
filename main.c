#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char * const argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Usage: %s [command [argument ...]]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

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

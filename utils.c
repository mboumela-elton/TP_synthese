#include <string.h>
#include <unistd.h>

#include "constant.c"

void printMessage(char *message) {
    write(STDOUT_FILENO, message, strlen(message));
}

ssize_t readCommandLine(char *command) {
    return read(STDIN_FILENO, command, sizeof(command) - 1);
}
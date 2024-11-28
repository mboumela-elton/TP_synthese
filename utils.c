#include <string.h>
#include <unistd.h>

#include "constant.c"

void printMessage(char *message) {
    write(STDOUT_FILENO, message, strlen(message));
}

ssize_t readCommandLine(char *command) {
    int bytesRead = read(STDIN_FILENO, command, sizeof(command) - 1);
    command[bytesRead - 1] = '\0';
    return bytesRead;
}
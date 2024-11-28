#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "constant.c"

void printMessage(char *message)
{
    write(STDOUT_FILENO, message, strlen(message));
}

ssize_t readCommandLine(char *command)
{
    return read(STDIN_FILENO, command, sizeof(command) - 1);
}

void execSingleCommand(char *command)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execlp(command, command, NULL);
        printMessage(COMMAND_ERROR);
        _exit(1);
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        printMessage(PROCESS_ERROR);
    }
}
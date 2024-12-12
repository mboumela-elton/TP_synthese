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

void intToStr(int num, char *str)
{
    int i = 0;
    int isNegative = 0;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }

    while (num != 0)
    {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }

    if (isNegative)
    {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void execSingleCommand(char *command)
{
    int status;
    pid_t pid = fork();
    if (pid == 0)
    {
        execlp(command, command, NULL);
        printMessage(COMMAND_ERROR);
        _exit(1);
    }
    else if (pid > 0)
    {
        wait(&status);
        char buffer[256];
        char numStr[12];

        if (WIFEXITED(status))
        {
            int exit_code = WEXITSTATUS(status);
            strcpy(buffer, "enseash [exit:");
            intToStr(exit_code, numStr);
            strcat(buffer, numStr);
            strcat(buffer, "] % ");
        }
        else if (WIFSIGNALED(status))
        {
            int signal_num = WTERMSIG(status);
            strcpy(buffer, "enseash [sign:");
            intToStr(signal_num, numStr);
            strcat(buffer, numStr);
            strcat(buffer, "] % ");
        }
        printMessage(buffer);
    }
    else
    {
        printMessage(PROCESS_ERROR);
    }
}

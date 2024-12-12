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

void prompt(int code, char *str, char *buffer, char *numStr)
{
    strcpy(buffer, str);
    intToStr(code, numStr);
    strcat(buffer, numStr);
    strcat(buffer, "] % ");
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
            prompt(WEXITSTATUS(status), PROMPT_EXIT, buffer, numStr);
        }
        else if (WIFSIGNALED(status))
        {
            prompt(WTERMSIG(status), PROMPT_SIGN, buffer, numStr);
        }
        printMessage(buffer);
    }
    else
    {
        printMessage(PROCESS_ERROR);
    }
}

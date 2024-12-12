#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

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

void prompt(int code, char *str, char *buffer, char *numStr, long timeTaken, char *timeStr)
{
    strcpy(buffer, str);
    intToStr(code, numStr);
    strcat(buffer, numStr);
    strcat(buffer, "|");
    intToStr(timeTaken, timeStr);
    strcat(buffer, timeStr);
    strcat(buffer, "ms] % ");
}

void execSingleCommand(char *command)
{
    int status;
    struct timespec start, end;
    pid_t pid = fork();
    if (pid == 0)
    {
        clock_gettime(CLOCK_REALTIME, &start);

        // Tokenize the command string into arguments
        char *args[256];
        int i = 0;
        args[i] = strtok(command, " ");
        while (args[i] != NULL)
        {
            i++;
            args[i] = strtok(NULL, " ");
        }
        // Check for redirection operators
        for (int j = 0; j < i; j++)
        {
            if (strcmp(args[j], ">") == 0)
            {
                int fd = open(args[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(fd, STDOUT_FILENO);
                close(fd);
                args[j] = NULL;
                break;
            }
            else if (strcmp(args[j], "<") == 0)
            {
                int fd = open(args[j + 1], O_RDONLY);
                dup2(fd, STDIN_FILENO);
                close(fd);
                args[j] = NULL;
                break;
            }
        }

        execvp(args[0], args);
        printMessage(COMMAND_ERROR);
        _exit(1);
    }
    else if (pid > 0)
    {
        wait(&status);
        clock_gettime(CLOCK_REALTIME, &end);
        long timeTaken = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
        char buffer[256];
        char numStr[12];
        char timeStr[12];

        if (WIFEXITED(status))
        {
            prompt(WEXITSTATUS(status), PROMPT_EXIT, buffer, numStr, timeTaken, timeStr);
        }
        else if (WIFSIGNALED(status))
        {
            prompt(WTERMSIG(status), PROMPT_SIGN, buffer, numStr, timeTaken, timeStr);
        }
        printMessage(buffer);
    }
    else
    {
        printMessage(PROCESS_ERROR);
    }
}

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

// convert int into char
void intToStr(int num, char *str)
{
    int i = 0;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    while (num != 0)
    {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
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

// syntaxe to print program code and time execution
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
    clock_gettime(CLOCK_MONOTONIC, &start);

    pid_t pid = fork();
    if (pid == 0)
    {
        // Tokenize the command string into arguments
        char *argv[256];
        int argc = 0;
        argv[argc] = strtok(command, " ");

        while (argv[argc] != NULL)
        {
            argc++;
            argv[argc] = strtok(NULL, " ");
        }
        // Check for redirection operators
        for (int j = 0; j < argc; j++)
        {
            if (strcmp(argv[j], ">") == 0)
            {
                // argv[j + 1] = 'file.txt';
                char *filename = argv[j + 1];
                printMessage(filename);
                int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(fd, STDOUT_FILENO);
                close(fd);
                argv[j] = NULL;
                break;
            }
            else if (strcmp(argv[j], "<") == 0)
            {
                int fd = open(argv[j + 1], O_RDONLY);
                dup2(fd, STDIN_FILENO);
                close(fd);
                argv[j] = NULL;
                break;
            }
        }

        execvp(argv[0], argv);
        printMessage(COMMAND_ERROR);
        _exit(1);
    }
    else if (pid > 0)
    {
        wait(&status);
        clock_gettime(CLOCK_MONOTONIC, &end);

        int timeTaken = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
        char buffer[BUFFER_SIZE];
        char numStr[STR_SIZE];
        char timeStr[STR_SIZE];

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

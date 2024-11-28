#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "utils.c"
#include "constant.c"

int enseash()
{
  char command[256];

  printMessage(WELCOME_MESSAGE);
  printMessage(EXIT_MESSAGE);

  while (1)
  {
    printMessage(ENSEASH);

    // Read the command entered by the user
    int bytes_read = readCommandLine(command);

    // Remove the newline character
    command[bytes_read - 1] = '\0';

    if (bytes_read <= 0)
    {
      continue;
    }

    // Check for the 'exit' command
    if (strcmp(command, EXIT) == 0)
    {
      break;
    }

    // Create a child process to execute the command
    pid_t pid = fork();
    if (pid == 0)
    {
      // Child process
      execlp(command, command, NULL);
      // If exec fails
      printMessage(COMMAND_ERROR);
      _exit(1);
    }
    else if (pid > 0)
    {
      // Parent process
      wait(NULL);
    }
    else
    {
      printMessage(PROCESS_ERROR);
    }
  }

  printMessage(GOODBYE_MESSAGE);
  return 0;
}
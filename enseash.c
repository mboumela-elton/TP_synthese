// #include <string.h>
// #include <unistd.h>

// int main() {
//     const char *welcome_msg = "Bienvenue dans le Shell ENSEA.\n";
//     write(STDOUT_FILENO, welcome_msg, strlen(welcome_msg));

//     while (1)
//     {
//       write(STDOUT_FILENO, welcome_msg, strlen(welcome_msg));

//     }

// }

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

    int bytesRead = readCommandLine(command);

    // Vérification de la commande 'exit'
    if (strcmp(command, EXIT) == 0 || bytesRead > 0)
    {
      // Création d'un processus enfant pour exécuter la commande
      pid_t pid = fork();
      if (pid == 0)
      {
        // Processus enfant
        execlp(command, command, (char *)NULL);
        write(STDERR_FILENO, "\nErreur d'exécution de la commande\n", 34);
        _exit(1);
      }
      else if (pid > 0)
      {
        // Processus parent
        wait(NULL);
      }
      else
      {
        // Échec du fork
        write(STDERR_FILENO, "Erreur de création du processus\n", 32);
      }
    }
  }

  printMessage(GOODBYE_MESSAGE);
  return 0;
}
// #include <stdlib.h>
// #include <stdio.h>
// #include <unistd.h>

// int main() {
//     pid_t pid = fork(); // Créer un processus fils

//     if (pid < 0) {
//         // Erreur lors de la création du processus
//         perror("Erreur lors du fork");
//         exit(1);
//     } else if (pid == 0) {
//         // Code exécuté par le fils
//         printf("PID du fils : %d\n", getpid());
//         printf("PID du père : %d\n", getppid());
//     } else {
//         // Code exécuté par le père
//         printf("PID du père : %d\n", getpid());
//         printf("PID du fils : %d\n", pid);
//     }

//     // Boucle infinie pour maintenir les processus actifs
//     while (1) {
//         // Attendre un signal ou faire d'autres tâches
//     }

//     return 0; // Ce point ne sera jamais atteint
// }

#include <stdio.h>
#include <string.h>

int main() {
    // Chaîne à tokeniser
    char str[] = "Bonjour, comment ça va?";
    
    // Délimiteurs : espace, virgule et point d'interrogation
    const char *delimiters = " ,?";

    // Utilisation de strtok pour obtenir le premier token
    char *token = strtok(str, delimiters);

    // Boucle pour récupérer tous les tokens
    while (token != NULL) {
        printf("Token: %s\n", token);
        // Obtenir le token suivant
        token = strtok(NULL, delimiters);
    }

    return 0;
}
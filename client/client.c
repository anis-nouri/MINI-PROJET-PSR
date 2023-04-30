#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8888

int main() {
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000], request[100];
    int choice = 0;

    // Créer un socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Impossible de créer le socket\n");
        return 1;
    }

    // Préparer l'adresse du serveur
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Établir la connexion avec le serveur
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Erreur de connexion\n");
        return 1;
    }

    printf("Connexion établie\n");
    

    // Menu
    while (choice != 4) {
        
        system("clear"); // clear the screen
        printf("======= MENU PRINCIPAL =======\n");
        printf("1. Consulter la liste des vols\n");
        printf("2. Consulter la facture d'une agence\n");
        printf("3. Consulter l'historique des transactions\n");
        printf("4. Quitter\n");

        printf("Votre choix : ");
        scanf("%d", &choice);

        system("clear"); // clear the screen again
        
         // Clear the buffer
         bzero(server_reply, sizeof(server_reply));


        // Traiter la réponse du serveur
        switch (choice) {
            case 1:
                printf("======= CONSULTER LA LISTE DES VOLS =======\n");
                printf("Référence du vol : ");
                scanf("%s", message);
                strncpy(request,"LISTE_VOL:",sizeof(request));
                strcat(request,message);  
                strcat(request, "\r");
                  printf("\n %s \n",request);     

                // Envoyer la référence au serveur
                if (send(sock, request, strlen(request), 0) < 0) {
                    printf("Erreur lors de l'envoi des données\n");
                    return 1;
                }

                // Recevoir la facture correspondante
                
                if (recv(sock, server_reply, 2000, 0) < 0) {
                    printf("Erreur lors de la réception des données\n");
                    return 1;
                }


                // Afficher la liste des vols
                printf("%s\n", server_reply);
                break;

            case 2:
                printf("======= CONSULTER LA FACTURE D'UNE AGENCE =======\n");
                // Demander la référence de l'agence
                printf("Référence de l'agence : ");
                scanf("%s", message);
                strncpy(request,"REF_AGE:",sizeof(request));
                strcat(request,message); 
                strcat(request, "\r");

                // Envoyer la référence au serveur
                if (send(sock, request, strlen(request), 0) < 0) {
                    printf("Erreur lors de l'envoi des données\n");
                    return 1;
                }

                // Recevoir la facture correspondante
                if (recv(sock, server_reply, 2000, 0) < 0) {
                    printf("Erreur lors de la réception des données\n");
                    return 1;
                }

                // Afficher la facture
                printf("%s\n", server_reply);
                break;

            case 3:
                printf("======= CONSULTER L'HISTORIQUE DES TRANSACTIONS =======\n");
                strncpy(request,"HIST_TR:",sizeof(request));
                strcat(request, "\r");

                // Envoyer la référence au serveur
                if (send(sock, request, strlen(request), 0) < 0) {
                    printf("Erreur lors de l'envoi des données\n");
                    return 1;
                }

                // Recevoir l'historique des transactions
                char buffer[4000];
                int bytes_received = 0;
                int total_bytes_received = 0;

                do {
                    bytes_received = recv(sock, buffer + total_bytes_received, sizeof(buffer) - total_bytes_received, 0);
                    if (bytes_received == -1) {
                        printf("Erreur lors de la réception des données\n");
                        return 1;
                    }
                    total_bytes_received += bytes_received;
                     // Check if the message ends with a newline character
                    if (total_bytes_received > 0 && buffer[total_bytes_received - 1] == '\n') {
                        break;
                    }
                } while (1);

                // Afficher l'historique des transactions

                printf("%s\n", buffer);

                break;

            case 4:
                // Quitter
                break;

            default:
                printf("Choix invalide\n");
                break;
        }
        if (choice != 4) {
            printf("Appuyez sur une touche pour continuer...\n");
            getchar();
            getchar();
        }
    }

    close(sock);
    return 0;
}

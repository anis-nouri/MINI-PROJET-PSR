#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define REF_AG 1

int main() {
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000], request[100];
    int choice = 0, option=0;
    int transaction_value = 0,ref_vol=0;

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
        printf("1. Réaliser des transactions\n");
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
                option =0;
                while (option != 3) {
                    system("clear"); // clear the screen again
                    printf("======= Réaliser des transactions =======\n");
                    printf("1. Effectuer une demande de réservation\n");
                    printf("2. Annuler une réservation\n");
                    printf("3. Retourner au menu principal\n");
                    printf("Entrez votre choix: ");
                    scanf("%d", &option);
                    system("clear");
                    // Clear the buffer
                     bzero(server_reply, sizeof(server_reply));
                    switch (option) {
                        case 1:
                            printf("======= Effectuer une demande de réservation =======\n");
                            char request[2000];
                            sprintf(request, "ALL_VOL:1");
                            strcat(request, "\r");     

                             // Send the request to the server
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
                            printf("%s",server_reply);
                            memset(server_reply, 0, sizeof(server_reply));
                            
                            printf("\nEntrez le numéro du vol: ");
                            scanf("%d", &ref_vol);

                            printf("Entrez le nombre de places demandées: ");
                            scanf("%d", &transaction_value);

                            
                            sprintf(request, "TRANSACTION:%d:%d:%s:%d", ref_vol,REF_AG, "Demande", transaction_value);
                            strcat(request, "\r");     

                            // Send the request to the server
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
                            printf("\nRESULTAT DE LA TRANSACTION ==> %s\n", server_reply);
                            

                            break;
                        case 2:
                            printf("Entrez le nombre de places à annuler: ");
                            scanf("%d", &transaction_value);

                            // code to handle reservation cancellation with transaction_value
                            break;
                        case 3:
                            printf("Retour au menu principal.\n");
                            break;
                        default:
                            printf("Choix invalide. Veuillez réessayer.\n");
                            break;
                    }
                    if (option != 3) {
                    printf("Appuyez sur une touche pour continuer...\n");
                    getchar();
                    getchar();
        }

                }

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

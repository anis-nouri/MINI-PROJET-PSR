#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8888

void send_request(int sock, const char *request, char *response)
{
    // Send the request to the server
    if (send(sock, request, strlen(request), 0) < 0)
    {
        printf("Erreur lors de l'envoi des données\n");
        return;
    }

    // Wait for a response from the server
    if (recv(sock, response, 2000, 0) < 0)
    {
        printf("Erreur lors de la réception des données\n");
        return;
    }
}

int main(int argc, char *argv[])
{
    int REF_AG = atoi(argv[1]);
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000], request[2000];
    float fact = 0;

    int choice = 0, option = 0;
    int transaction_value = 0, ref_vol = 0;

    // Créer un socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Impossible de créer le socket\n");
        return 1;
    }

    // Préparer l'adresse du serveur
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Établir la connexion avec le serveur
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Erreur de connexion\n");
        return 1;
    }

    printf("Connexion établie\n");

    // Menu
    while (choice != 3)
    {

        system("clear"); // clear the screen
        printf("======= MENU PRINCIPAL AGENCE %d =======\n\n",REF_AG);
        printf("1. Réaliser des transactions\n");
        printf("2. Consulter la facture\n");
        printf("3. Quitter\n\n");

        printf("Votre choix : ");
        scanf("%d", &choice);

        system("clear"); // clear the screen again

        // Clear the buffer
        bzero(server_reply, sizeof(server_reply));

        // Traiter la réponse du serveur
        switch (choice)
        {
        case 1:
            option = 0;
            float fact_init;
            float fact_fin;

            // Préparer la requête pour obtenir la liste des vols disponibles
            sprintf(request, "GET_FAC:%d", REF_AG);
            strcat(request, "\r");

            // Envoyer la requête et attendre la réponse
            send_request(sock, request, server_reply);

            fact_init = atof(server_reply);

            while (option != 3)
            {
                system("clear"); // clear the screen again
                printf("======= Réaliser des transactions =======\n\n");
                printf("1. Effectuer une demande de réservation\n");
                printf("2. Annuler une réservation\n");
                printf("3. Retourner au menu principal\n\n");
                printf("Entrez votre choix: ");
                scanf("%d", &option);
                system("clear");
                // Clear the buffer
                bzero(server_reply, sizeof(server_reply));
                switch (option)
                {
                case 1:
                    // Demander une réservation
                    printf("======= Effectuer une demande de réservation =======\n");

                    // Préparer la requête pour obtenir la liste des vols disponibles
                    sprintf(request, "ALL_VOL:1");
                    strcat(request, "\r");

                    // Envoyer la requête et attendre la réponse
                    send_request(sock, request, server_reply);

                    // Afficher la liste des vols disponibles
                    printf("%s", server_reply);

                    // Vider la mémoire tampon pour stocker la réponse du serveur
                    memset(server_reply, 0, sizeof(server_reply));

                    // Demander le numéro de vol et le nombre de places demandées
                    printf("\nEntrez le numéro du vol: ");
                    scanf("%d", &ref_vol);

                    printf("Entrez le nombre de places demandées: ");
                    scanf("%d", &transaction_value);

                    // Préparer la requête pour effectuer la transaction
                    sprintf(request, "TRANSACTION:%d:%d:%s:%d", ref_vol, REF_AG, "Demande", transaction_value);
                    strcat(request, "\r");

                    // Envoyer la requête et attendre la réponse
                    send_request(sock, request, server_reply);

                    // Afficher le résultat de la transaction
                    printf("\nRESULTAT DE LA TRANSACTION ==> %s\n", server_reply);
                    break;
                case 2:
                    // Demander une réservation
                    printf("======= Effectuer une annulation de réservation =======\n");
                    // Préparer la requête pour obtenir la liste des vols disponibles
                    sprintf(request, "ALL_VOL:1");
                    strcat(request, "\r");

                    // Envoyer la requête et attendre la réponse
                    send_request(sock, request, server_reply);

                    // Afficher la liste des vols disponibles
                    printf("%s", server_reply);

                    // Vider la mémoire tampon pour stocker la réponse du serveur
                    memset(server_reply, 0, sizeof(server_reply));

                    // Demander le numéro de vol et le nombre de places demandées
                    printf("\nEntrez le numéro du vol: ");
                    scanf("%d", &ref_vol);

                    printf("Entrez le nombre de places à annulées: ");
                    scanf("%d", &transaction_value);

                    // Préparer la requête pour effectuer la transaction
                    sprintf(request, "TRANSACTION:%d:%d:%s:%d", ref_vol, REF_AG, "Annulation", transaction_value);
                    strcat(request, "\r");

                    // Envoyer la requête et attendre la réponse
                    send_request(sock, request, server_reply);

                    // Afficher le résultat de la transaction
                    printf("\nRESULTAT DE LA TRANSACTION ==> %s\n", server_reply);

                    break;
                case 3:
                    // Préparer la requête pour obtenir la facture finale
                    sprintf(request, "GET_FAC:%d", REF_AG);
                    strcat(request, "\r");

                    // Envoyer la requête et attendre la réponse
                    send_request(sock, request, server_reply);

                    // Récupérer la facture finale et afficher le résultat de la transaction
                    fact_fin = atof(server_reply);
                    float transaction_value = fact_fin - fact_init;
                    printf("\n==================================================\n");
                    printf("                 RÉSULTAT DE LA TRANSACTION\n");
                    printf("==================================================\n");
                    printf(" La valeur des transactions effectuées est : %.2f\n", transaction_value);
                    printf("==================================================\n\n");

                    // Afficher un message de confirmation et retourner au menu principal
                    printf("Retour au menu principal.\n");
                    break;
                default:
                    printf("Choix invalide. Veuillez réessayer.\n");
                    break;
                }
                if (option != 3)
                {
                    printf("Appuyez sur une touche pour continuer...\n");
                    getchar();
                    getchar();
                }
            }
            break;

        case 2:
            system("clear"); // clear the screen again
            sprintf(request, "GET_FAC:%d", REF_AG);
            strcat(request, "\r");

            // Envoyer la requête et attendre la réponse
            send_request(sock, request, server_reply);

            // Récupérer la facture finale et afficher le résultat de la transaction
            fact = atof(server_reply);

            printf("\n==================================================\n");
            printf("                 FACTURE FINALE\n");
            printf("==================================================\n");
            printf(" Le montant de votre facture est : %.2f\n", fact);
            printf("==================================================\n\n");
            break;
        case 3:
            break;

        default:
            printf("Choix invalide\n");
            break;
        }
        if (choice != 3)
        {
            printf("Appuyez sur une touche pour continuer...\n");
            getchar();
            getchar();
        }
    }

    close(sock);
    return 0;
}

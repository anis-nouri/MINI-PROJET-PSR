#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../include/reservations.h"
#include "../include/facture.h"
#include "../include/vols.h"

#define PORT 8888

int main() {
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000];
    char command[20];
    char transaction_type[20];
    int arg_int;
    int ref_vol, ref_agence, transaction_value;


    // Créer un socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Impossible de créer le socket\n");
        return 1;
    }

    // Préparer l'adresse du serveur
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Associer l'adresse au socket
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Erreur de bind\n");
        return 1;
    }

    // Mettre le socket en mode écoute
    listen(socket_desc, 3);

    printf("En attente de connexions entrantes...\n");

    // Accepter les connexions entrantes
    c = sizeof(struct sockaddr_in);
    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))) {
        printf("Connexion acceptée\n");

        // Recevoir des données du client
        while ((read_size = recv(client_sock, client_message, sizeof(client_message), 0)) > 0) {
            sscanf(client_message, "%[^:]", command);
            if(strcmp(command,"TRANSACTION")==0){
            // Extraire la commande et les arguments
            sscanf(client_message, "%[^:]:%d:%d:%[^:]:%d", command, &ref_vol, &ref_agence, transaction_type, &transaction_value);
            }
            else{
            char* colon_pos = strchr(client_message, ':');

            if (colon_pos != NULL) {
                // Extract the string after ':'
                char* arg = colon_pos + 1;

                // Convert the argument to an integer
                arg_int = atoi(arg);
            }
            }

            if (strcmp(command, "REF_AGE") == 0) {
                printf("**CONSULTER_FACTURE_AGE: [ref.age=%d]**\n",arg_int);
                afficher_facture(arg_int);
                envoyer_facture1(client_sock,arg_int);

            } else if (strcmp(command, "LISTE_VOL") == 0) {
                envoyer_info_vol(client_sock,arg_int);

            } else if (strcmp(command, "HIST_TR") == 0) {
                printf("**CONSULTER_HISTO_TRANSACTIONS\n");
                envoyer_histo(client_sock);

            }
            else if (strcmp(command, "GET_FAC") == 0) {
                printf("**CONSULTER_FACTURE_AGE: [ref.age=%d]**\n",arg_int);
                afficher_facture(arg_int);
                envoyer_facture2(client_sock,arg_int);

            }else if (strcmp(command, "TRANSACTION") == 0) {
                printf("**L'agence %d a effectué une transaction de type %s**\n", ref_agence, transaction_type);
                ajouter_transaction(client_sock,ref_vol,ref_agence, transaction_type,transaction_value);
            }
            else if (strcmp(command, "ALL_VOL") == 0) {
                envoyer_liste_vols(client_sock);
            }else {
                printf("Invalid command\n");
            }
            
            // Clear the buffer for the next message
            memset(client_message, 0, sizeof(client_message));
        }

        // Fermer la connexion avec le client
        close(client_sock);
    }

    if (client_sock < 0) {
        printf("Erreur de connexion avec le client\n");
        return 1;
    }

    return 0;
}
//NB: gcc main.c reservations.c facture.c vols.c -o main (on doit inclure les autres sources de fichiers reservations et tout
// dans la commande de compilation)


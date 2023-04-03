#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../include/reservations.h"


#define PORT 8888

int main() {
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000];
    char command[20];

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
        while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0) {
           // Receive data from the client
            memset(client_message, 0, sizeof(client_message)); // clear the buffer
            if ((read_size = recv(client_sock, client_message, sizeof(client_message), 0)) > 0) {
                // Process the received message
                printf("Message du client : %s\n", client_message);
                sscanf(client_message, "%[^:]", command);
                 if (strcmp(command, "REF_AGE") == 0) {
                    printf("REF_AGE command\n");
                } else if (strcmp(command, "LISTE_VOL") == 0) {
                    printf("LISTE_VOL command\n");
                } else if (strcmp(command, "HIST_TR") == 0) {
                    printf("HIST_TR command\n");
                    envoyer_histo(client_sock);
                } else {
                    printf("Invalid command\n");
                }
                }

            // Envoyer une réponse au client
            //write(client_sock, "Message reçu", strlen("Message reçu"));
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

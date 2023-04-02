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
    char message[1000], server_reply[2000];

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

    // Envoyer des données au serveur
    printf("Message à envoyer : ");
    fgets(message, 1000, stdin);
    if (send(sock, message, strlen(message), 0) < 0) {
        printf("Erreur lors de l'envoi des données\n");
        return 1;
    }

    // Recevoir la réponse du serveur
    if (recv(sock, server_reply, 2000, 0) < 0) {
        printf("Erreur lors de la réception de données\n");
        return 1;
    }

    printf("Réponse du serveur : %s\n", server_reply);

    close(sock);
    return 0;
}

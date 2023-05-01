#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "../include/reservations.h"
#include "../include/facture.h"
#include "../include/vols.h"

#define PORT 8888


//mutex pour le controle d'acces aux fichiers au mm temps: synchronisation
pthread_mutex_t vols_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t histo_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t facture_mutex=PTHREAD_MUTEX_INITIALIZER;




//*****************************************CODE WITH THREADS&MUTEX******************************


//la fonction qui va traiter les demandes des clients et envoyer la réponse aux clients
void *traitement_demande(void *);


int main()
{
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;


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

    // Mettre le socket en mode écoute-mode passive
    listen(socket_desc, 5);
    printf("En attente de connexions entrantes...\n");
     // Accepter les connexions entrantes
    c = sizeof(struct sockaddr_in);
  while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        printf("Connexion acceptée\n");

        pthread_t threadServer;
        // new_soc est une file contenant la liste des clients connectés
        int *new_sock = malloc(1);
        *new_sock = client_sock;


        if (pthread_create(&threadServer, NULL, traitement_demande, (void *)new_sock) < 0) {
            printf("Erreur de création de thread\n");
            return 1;
        }
    }


    if (client_sock < 0) {
        printf("Erreur de connexion avec le client\n");
        return 1;
    }

    //Destruction du mutex
     pthread_mutex_destroy(&vols_mutex);
     pthread_mutex_destroy(&facture_mutex);
     pthread_mutex_destroy(&histo_mutex);


    

    return 0;
}


void *traitement_demande(void *socket_desc) {
    int client_sock = *(int *)socket_desc;
    int read_size;
    char client_message[2000];
    char command[20];
    char transaction_type[20];
    int arg_int;
    int ref_vol, ref_agence, transaction_value;
    

    // Receive data from client
    while ((read_size = recv(client_sock, client_message, sizeof(client_message), 0)) > 0) {
        sscanf(client_message, "%[^:]", command);
        if(strcmp(command,"TRANSACTION")==0){
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
            pthread_mutex_lock(&facture_mutex);
            afficher_facture(arg_int);
            envoyer_facture(client_sock,arg_int);
            pthread_mutex_unlock(&facture_mutex);



        } else if (strcmp(command, "LISTE_VOL") == 0) {
            pthread_mutex_lock(&vols_mutex);
            envoyer_info_vol(client_sock,arg_int);
            pthread_mutex_unlock(&vols_mutex);



        } else if (strcmp(command, "HIST_TR") == 0) {
            printf("**CONSULTER_HISTO_TRANSACTIONS\n");
            pthread_mutex_lock(&histo_mutex);
            envoyer_histo(client_sock);
            pthread_mutex_unlock(&histo_mutex);



        }else if (strcmp(command, "TRANSACTION") == 0) {
            printf("**L'agence %d a effectué une transaction de type %s**\n", ref_agence, transaction_type);
            pthread_mutex_lock(&histo_mutex);
            ajouter_transaction(client_sock,ref_vol,ref_agence, transaction_type,transaction_value);
            pthread_mutex_unlock(&histo_mutex);

        }
        else if (strcmp(command, "ALL_VOL") == 0) {
         pthread_mutex_lock(&vols_mutex);
         envoyer_liste_vols(client_sock);
         pthread_mutex_unlock(&vols_mutex);

        }else {
            printf("Commande invalide\n");
        }


        // Clear le buffer pour le prochain message
        memset(client_message, 0, sizeof(client_message));
    }


    if (read_size == 0) {
        printf("Client deconnecté\n");
    } else if (read_size == -1) {
        printf("Erreur de réception des données\n");
    }
    close(client_sock);
    // Free the socket pointer
    free(socket_desc);


    return NULL;
}
















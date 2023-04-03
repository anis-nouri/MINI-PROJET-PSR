#ifndef RESERVATIONS_H
#define RESERVATIONS_H

struct reservation {
    int ref_vol;
    int agence;
    char transaction[20];
    int valeur;
    char resultat[20];
};

void afficher_histo();
void envoyer_histo(int client_sock) ;
int rechercher_reservation(int ref_vol, int agence);
void ajouter_transaction(int ref_vol, int agence, char* transaction, int valeur);
#endif

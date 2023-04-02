#ifndef RESERVATIONS_H
#define RESERVATIONS_H

struct reservation {
    int ref_vol;
    int agence;
    char transaction[20];
    int valeur;
    char resultat[20];
};

void afficher_histo(char* filename);
int rechercher_reservation(char* filename, int ref_vol, int agence);
int reserver(char* filename, int ref_vol, int agence, int nb_places);

#endif

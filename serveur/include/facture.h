#ifndef FACTURE_H
#define FACTURE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int ref_agence;      // référence de l'agence
    float somme_a_payer; // somme à payer par l'agence
} facture;

float afficher_facture(int);
void ajouterOuModifierFacture(int, float);
void envoyer_facture1(int client_sock, int ref_agence);
void envoyer_facture2(int client_sock, int ref_agence);

#endif

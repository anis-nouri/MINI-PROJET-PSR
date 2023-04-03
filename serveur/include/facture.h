#ifndef FACTURE_H
#define FACTURE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int ref_agence;      // référence de l'agence
    int somme_a_payer;   // somme à payer par l'agence
} facture;

float afficher_facture(int);
void ajouterOuModifierFacture(int, float);

#endif

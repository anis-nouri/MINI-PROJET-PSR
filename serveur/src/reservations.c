#include "../include/reservations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void afficher_histo(char* filename) {
    FILE* fp;
    struct reservation res;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return;
    }

    printf("Référence vol\tAgence\tTransaction\tValeur\tRésultat\n");
    while (fscanf(fp, "%d %d %s %d %s", &res.ref_vol, &res.agence, res.transaction, &res.valeur, res.resultat) != EOF) {
        printf("%d\t\t%d\t%s\t\t%d\t%s\n", res.ref_vol, res.agence, res.transaction, res.valeur, res.resultat);
    }

    fclose(fp);
}


int main(){
    // Affichage de l'historique complet
    afficher_histo("./BD/histo.txt");
    return 0;
}

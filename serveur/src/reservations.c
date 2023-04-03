#include "../include/reservations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/vols.h"

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


void consulter_historique_agence(char* filename, int agence) {
    FILE* fp;
    struct reservation res;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return;
    }

    printf("Référence vol\tAgence\tTransaction\tValeur\tRésultat\n");
    while (fscanf(fp, "%d %d %s %d %s", &res.ref_vol, &res.agence, res.transaction, &res.valeur, res.resultat) != EOF) {
        if (res.agence == agence) {
            printf("%d\t\t%d\t%s\t\t%d\t%s\n", res.ref_vol, res.agence, res.transaction, res.valeur, res.resultat);
        }
    }

    fclose(fp);
}

void ajouter_transaction(char* filename, int ref_vol, int agence, char* transaction, int valeur) {
    FILE* fp;
    struct reservation res;
    int nb_places = get_nb_places(ref_vol);
    char resultat[20] = "";
    
    if(valeur > nb_places && strcmp(transaction, "Demande") == 0) {
        strcpy(resultat, "impossible");
    }
    else if(strcmp(transaction, "Annulation") == 0 || (valeur <= nb_places && strcmp(transaction, "Demande") == 0)) {
        strcpy(resultat, "succes");
        if(strcmp(transaction, "Annulation")==0){
            modifier_nbplaces(ref_vol,nb_places+valeur);
        }
        else if(strcmp(transaction, "Demande")==0){
            modifier_nbplaces(ref_vol,nb_places-valeur);
        }
    }
    
    fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return;
    }

    res.ref_vol = ref_vol;
    res.agence = agence;
    strcpy(res.transaction, transaction);
    res.valeur = valeur;
    strcpy(res.resultat, resultat);

    fprintf(fp, "%d %d %s %d %s\n", res.ref_vol, res.agence, res.transaction, res.valeur, res.resultat);

    fclose(fp);
}

int main(){
    // Affichage de l'historique complet
    afficher_histo("./BD/histo.txt");
    consulter_historique_agence("./BD/histo.txt", 2);
    printf("%d",get_nb_places(3000));
    
    /*
    ajouter_transaction("./BD/histo.txt", 2000, 2, "Demande", 10);
    ajouter_transaction("./BD/histo.txt", 1000, 1, "Demande", 25);
    ajouter_transaction("./BD/histo.txt", 2000, 2, "Demande", 10);
    ajouter_transaction("./BD/histo.txt", 1000, 2, "Demande", 10);
    ajouter_transaction("./BD/histo.txt", 2000, 1, "Demande", 5);
    ajouter_transaction("./BD/histo.txt", 3000, 2, "Demande", 45);
    ajouter_transaction("./BD/histo.txt", 1000, 2, "Annulation", 5);
    ajouter_transaction("./BD/histo.txt", 4000, 1, "Demande", 15);
    */


    return 0;
}

#include "../include/reservations.h"
#include "../include/vols.h"
#include "../include/facture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define filename "./BD/histo.txt"

void envoyer_histo(int client_sock) {

    FILE* fp;
    struct reservation res;
    char buffer[2000];

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return;
    }

    sprintf(buffer, "Référence vol\tAgence\tTransaction\tValeur\tRésultat\n");
    write(client_sock, buffer, strlen(buffer));

    while (fscanf(fp, "%d %d %s %d %s", &res.ref_vol, &res.agence, res.transaction, &res.valeur, res.resultat) != EOF) {
        sprintf(buffer, "%d\t\t%d\t%s\t\t%d\t%s\n", res.ref_vol, res.agence, res.transaction, res.valeur, res.resultat);
        write(client_sock, buffer, strlen(buffer));
    }
    

    fclose(fp);
}



void afficher_histo() {
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


void consulter_historique_agence(int agence) {
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

void ajouter_transaction(int client_sock, int ref_vol, int agence, char* transaction, int valeur) {
    FILE* fp;
    struct reservation res;
    int nb_places = get_nb_places(ref_vol);
    float prix_place = get_prix_place(ref_vol);
    float prix_facture = afficher_facture(agence);
    float nouveau_prix_facture=0;
    char resultat[20] = "";
    
    if(valeur > nb_places && strcmp(transaction, "Demande") == 0) {
        strcpy(resultat, "impossible");
    }
    else if(strcmp(transaction, "Annulation") == 0 || (valeur <= nb_places && strcmp(transaction, "Demande") == 0)) {
        strcpy(resultat, "succes");
        if(strcmp(transaction, "Annulation")==0){
            nouveau_prix_facture=prix_facture-(valeur*prix_place)+0.1*(valeur*prix_place);
            modifier_nbplaces(ref_vol,nb_places+valeur);
            ajouterOuModifierFacture(agence, nouveau_prix_facture);
        }
        else if(strcmp(transaction, "Demande")==0){
            nouveau_prix_facture=prix_facture+(valeur*prix_place);
            modifier_nbplaces(ref_vol,nb_places-valeur);
            ajouterOuModifierFacture(agence, nouveau_prix_facture);

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
    
        char message[100];
    if (strcmp(resultat, "succes") == 0) {
        snprintf(message, sizeof(message), "Transaction effectuée avec succès !\n");
    } else {
        snprintf(message, sizeof(message), "Transaction impossible !\n");
    }

    write(client_sock, message, strlen(message));

    fclose(fp);
}

/*
int main(){
    ajouter_transaction(2000, 2, "Demande", 10);
    ajouter_transaction(1000, 1, "Demande", 25);
    ajouter_transaction(2000, 2, "Demande", 10);
    ajouter_transaction(1000, 2, "Demande", 10);
    ajouter_transaction(2000, 1, "Demande", 5);
    ajouter_transaction(3000, 2, "Demande", 45);
    ajouter_transaction(1000, 2, "Annulation", 5);
    ajouter_transaction(4000, 1, "Demande", 15);
    
    // Affichage de l'historique complet
    afficher_histo();

    // Affichage de l'historique de l'agence 2
    consulter_historique_agence(2);
    return 0;
}*/


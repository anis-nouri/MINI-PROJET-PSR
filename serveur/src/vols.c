#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/vols.h"
#define MAX_VOLS 100

void affichervols() {
    vol vol1;
    FILE *fichier = fopen("../BD/vols.txt", "r");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    //fscanf(fichier, "%*s %*s %*s %*s"); // Ignorer la première ligne contenant les noms de colonnes
    printf("Référence \tDestination \tNombre de places \tPrix par place\n");
    while (fscanf(fichier, "%d %s %d %f", &vol1.reference, vol1.destination, &vol1.nombre_places, &vol1.prix_place) != EOF) {
        printf("%d\t\t%s\t\t%d\t\t%.2f\n", vol1.reference, vol1.destination, vol1.nombre_places, vol1.prix_place);
    }
    
    fclose(fichier);
}

void modifier_nbplaces(int reference, int nouveau_nombre_places) {
    vol vols[MAX_VOLS];
    FILE *fichier = fopen("./BD/vols.txt", "r+");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // lecture des vols
    int nb_vols = 0;
    char ligne[100];
    while (fgets(ligne, 100, fichier) != NULL) {
        if (sscanf(ligne, "%d %s %d %f", &vols[nb_vols].reference, vols[nb_vols].destination, &vols[nb_vols].nombre_places, &vols[nb_vols].prix_place) != 4) {
            fprintf(stderr, "Erreur de format de la ligne: %s\n", ligne);
            exit(EXIT_FAILURE);
        }
        nb_vols++;
    }

    // recherche du vol à modifier
    int i;
    for (i = 0; i < nb_vols; i++) {
        if (vols[i].reference == reference) {
            vols[i].nombre_places = nouveau_nombre_places;
            break;
        }
    }

    // réécriture des vols dans le fichier
    rewind(fichier);
    for (i = 0; i < nb_vols; i++) {
        fprintf(fichier, "%d %s %d %.2f\n", vols[i].reference, vols[i].destination, vols[i].nombre_places, vols[i].prix_place);
    }

    fclose(fichier);
}


int main()
{
   //affichervols(); 
   modifier_nbplaces(3000,20);

   return 0; 
}
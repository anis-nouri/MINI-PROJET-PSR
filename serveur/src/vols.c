#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/vols.h"
#define MAX_VOLS 100
#define filename "./BD/vols.txt"


void affichervols() {
    vol vol1;
    FILE *fichier = fopen(filename, "r");

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
    FILE *fichier = fopen(filename, "r+");
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
        fprintf(fichier, "%d %s %d %.2f", vols[i].reference, vols[i].destination, vols[i].nombre_places, vols[i].prix_place);
        if(i != nb_vols-1) {
            fprintf(fichier, "\n");
        }
    }

    fclose(fichier);
}


int get_nb_places(int ref_vol) {
    FILE* fp;
    vol v;
    int nb_places = -1;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier vols.txt\n");
        return -1;
    }

    while (fscanf(fp, "%d %s %d %f", &v.reference, v.destination, &v.nombre_places, &v.prix_place) != EOF) {
        if (v.reference == ref_vol) {
            nb_places = v.nombre_places;
            break;
        }
    }
    fclose(fp);

    if (nb_places == -1) {
        printf("Le vol avec la référence %d n'a pas été trouvé dans le fichier vols.txt\n", ref_vol);
    }

    return nb_places;
}

float get_prix_place(int ref_vol) {
    FILE* fp;
    vol v;
    float prix_place = -1;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier vols.txt\n");
        return -1;
    }

    while (fscanf(fp, "%d %s %d %f", &v.reference, v.destination, &v.nombre_places, &v.prix_place) != EOF) {
        if (v.reference == ref_vol) {
            prix_place = v.prix_place;
            break;
        }
    }
    fclose(fp);

    if (prix_place == -1) {
        printf("Le vol avec la référence %d n'a pas été trouvé dans le fichier vols.txt\n", ref_vol);
    }

    return prix_place;
}


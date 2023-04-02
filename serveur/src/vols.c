#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vol {
    int ref;
    char dest[50];
    int places_dispo;
    int prix_place;
};

void consulter_vols(char* filename) {
    FILE* fp;
    struct vol v;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return;
    }

    printf("Référence\tDestination\tPlaces dispo.\tPrix/place\n");
    while (fscanf(fp, "%d %s %d %d", &v.ref, v.dest, &v.places_dispo, &v.prix_place) != EOF) {
        printf("%d\t%s\t\t%d\t\t%d\n", v.ref, v.dest, v.places_dispo, v.prix_place);
    }

    fclose(fp);
}

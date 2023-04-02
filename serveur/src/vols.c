#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/vols.h"


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
int main()
{
   affichervols(); 

   return 0; 
}
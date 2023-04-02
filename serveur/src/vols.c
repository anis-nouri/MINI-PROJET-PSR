#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/vols.h"


void afficherVols() {
    FILE *fichier = fopen("../BD/vols.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    
    Vol vol;
    fscanf(fichier, "%*s %*s %*s %*s"); // Ignorer la première ligne contenant les noms de colonnes
    
    while (fscanf(fichier, "%d %s %d %f", &vol.reference, vol.destination, &vol.nombrePlaces, &vol.prixPlace) != EOF) {
        printf("Référence : %d\nDestination : %s\nNombre de places : %d\nPrix par place : %.2f\n\n", vol.reference, vol.destination, vol.nombrePlaces, vol.prixPlace);
    }
    
    fclose(fichier);
}
int main()
{
   afficherVols(); 

   return 0; 
}
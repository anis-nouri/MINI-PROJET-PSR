#ifndef VOLS_H
#define VOLS_H

// Structure pour représenter un vol
typedef struct {
    int reference;
    char destination[20];
    int nombre_places;
    float prix_place;
} vol;

// Fonction pour récupérer tous les vols du fichier texte "vols.txt" et les afficher à la console
void afficher_vols();
void modifier_nbplaces(int,int);

#endif /* VOL_H */

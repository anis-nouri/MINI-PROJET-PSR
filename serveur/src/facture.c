#include "../include/facture.h"
#include "../include/vols.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>


#define filename "./BD/facture.txt"

float afficher_facture(int ref_agence) {
    FILE* fp;
    fp = fopen(filename, "r");
    facture f;
    int trouve = 0;
    while (fscanf(fp, "%d %f", &f.ref_agence, &f.somme_a_payer) == 2) {
        if (f.ref_agence == ref_agence) {
            trouve = 1;
            return f.somme_a_payer;
        }
    }
    if (!trouve) {
        return 0;
    }
    rewind(fp);
}

void envoyer_facture(int client_sock, int ref_agence) {
    FILE* fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Unable to open file");
        return;
    }
    facture f;
    int trouve = 0;
    while (fscanf(fp, "%d %f", &f.ref_agence, &f.somme_a_payer) != EOF) {
        if (f.ref_agence == ref_agence) {
            trouve = 1;
            char facture_str[100];
            sprintf(facture_str, "\nFacture de l'agence %d : %.2f euros\n", ref_agence, f.somme_a_payer);
            ssize_t bytes_written = write(client_sock, facture_str, strlen(facture_str));
        }
    }
    if (!trouve) {
        write(client_sock, "\nFacture non trouvée !!\n", strlen("\nFacture non trouvée !!\n"));
    }
    rewind(fp);
}



void ajouterOuModifierFacture(int referenceAgence, float nouvelleSomme)
{
    // Ouvrir le fichier en mode lecture
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    // Ouvrir un fichier temporaire en mode écriture
    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erreur: Impossible de créer le fichier temporaire\n");
        fclose(file);
        return;
    }

    int reference;
    float somme;
    bool found = false;

    // Parcourir le fichier ligne par ligne
    while (fscanf(file, "%d %f", &reference, &somme) == 2) {
        if (reference == referenceAgence) {
            // Modifier la facture
            fprintf(tempFile, "%d %.2f\n", reference, nouvelleSomme);
            found = true;
        } else {
            // Recopier la facture telle quelle
            fprintf(tempFile, "%d %.2f\n", reference, somme);
        }
    }

    // Si la facture n'a pas été trouvée, l'ajouter à la fin du fichier
    if (!found) {
        fprintf(tempFile, "%d %.2f\n", referenceAgence, nouvelleSomme);
    }

    // Fermer les fichiers
    fclose(file);
    fclose(tempFile);

    // Renommer le fichier temporaire pour remplacer l'original

    remove(filename);
    rename("temp.txt", filename);
}

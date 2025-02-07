#include "../headers/tourelles.h"
#include "../headers/etudiant.h"
#include "stdlib.h"
#include "stdio.h"

#include <windows.h>

#define LIGNES 7
#define COLONNES 15


Tourelle* ajouter_tourelle(Tourelle *tete, int ligne, int colonne, char type) {
    Tourelle *nouvelle_tourelle = malloc(sizeof(Tourelle));
    if (nouvelle_tourelle == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    nouvelle_tourelle->ligne = ligne;
    nouvelle_tourelle->colonne = colonne;
    nouvelle_tourelle->type = type;

    nouvelle_tourelle->suivant = tete; // Ajoute en tête de liste

    //on utilise un switch pour les diff types de tourelles selon leurs types

    switch (type) {
        case 'T': // Tourelle basique
            nouvelle_tourelle->vie = 5;
            nouvelle_tourelle->degats = 2;
            nouvelle_tourelle->portee = 3;
            //prix de 100
            break;
        case 'L': // Tourelle longue portée
            nouvelle_tourelle->vie = 4;
            nouvelle_tourelle->degats = 1;
            nouvelle_tourelle->portee = 5;
            //prix de 200
            break;
        case 'P': // Tourelle puissante
            nouvelle_tourelle->vie = 6;
            nouvelle_tourelle->degats = 4;
            nouvelle_tourelle->portee = 2;
            //prix de 300
            break;

        default:
            printf("Type de tourelle inconnu : %c\n", type);
            free(nouvelle_tourelle);
            return NULL;
    }
    
    return nouvelle_tourelle;
}


void attaquer_ennemis(char grille[LIGNES][COLONNES], Tourelle *tourelles, Etudiant *ennemis, int* cagnotte) { //tourelles->>>>>etudiants
    Tourelle *courant_t = tourelles;

    while (courant_t != NULL) { 

        for (int c = 1; c <= courant_t->portee; c++) {
            
            int colonne_cible = courant_t->colonne + c;

            Etudiant *courant_e = ennemis;

            while (courant_e != NULL) { //on parcourt toute la liste chainée d'étudiant

                if (courant_e->actif && courant_e->ligne == courant_t->ligne && courant_e->colonne == colonne_cible) {

                    courant_e->pointsDeVie-=courant_t->degats;

                    if (courant_e->pointsDeVie<=0){
                        courant_e->actif = 0; // desactive l'ennemi => le supppr de l'affichage
                        //A FAIRE supprimer_enenemi(courant_e)
                        //A FAIRE rajouter de l'argent a la cagnotte
                        grille[courant_e->ligne - 1][courant_e->colonne] = '_'; // Efface l'ennemi de la grille car remplacer par un _
                        ennemis=supprimer_ennemi(ennemis, courant_e);


                        int gain = 0;
                        switch (courant_e->type) {
                            case 'Z': gain = 50; break;
                            case 'A': gain = 60; break;
                            case 'S': gain = 100; break;
                            case 'M': gain = 200; break;
                            case 'V': gain = 75; break;
                            default: gain = 50; // au cas ou on mets un ptit default
                        }

                        *cagnotte += gain;
                        printf("\033[33m Tu viens de gagner %d pièces d'or \033[0m \n", gain);

                    }
 
                    break;
                }

                courant_e = courant_e->suivant;
            }
        }
        courant_t = courant_t->suivant;
    }

}

void attaquer_tourelles(Tourelle **tourelles, Etudiant *ennemi_bloquant) {
    if (ennemi_bloquant == NULL) return; // Aucun ennemi ne doit attaquer

    Tourelle *courant_t = *tourelles;
    while (courant_t != NULL) {
        if (courant_t->ligne == ennemi_bloquant->ligne && 
            courant_t->colonne == ennemi_bloquant->colonne - 1) {

            printf("L'ennemi [%c] en (%d, %d) attaque la tourelle en (%d, %d) !\n",
                   ennemi_bloquant->type, ennemi_bloquant->ligne, ennemi_bloquant->colonne,
                   courant_t->ligne, courant_t->colonne);

            courant_t->vie -= ennemi_bloquant->degats;

            if (courant_t->vie <= 0) {
                *tourelles = supprimer_tourelle(*tourelles, courant_t->ligne, courant_t->colonne);
            }
            return;
        }
        courant_t = courant_t->suivant;
    }
}





Tourelle* supprimer_tourelle(Tourelle *tete, int ligne, int colonne) { //pour la mémoire
    Tourelle *courant = tete;
    Tourelle *precedent = NULL;

    while (courant != NULL) {

        if (courant->ligne == ligne && courant->colonne == colonne) {

            if (precedent == NULL) {  
                Tourelle *temp = courant->suivant;
                free(courant);
                return temp;
            } 
            
            else {
                precedent->suivant = courant->suivant;
                free(courant);
                return tete;
            }

        }

        precedent = courant;
        courant = courant->suivant;
    }

    return tete;

}


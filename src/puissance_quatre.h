#ifndef PUISSANCE_QUATRE_H
#define PUISSANCE_QUATRE_H

#define NB_LIGNE 6
#define NB_COLONNE 7

typedef enum
{
    VIDE,
    J1,
    J2
} Case;

typedef Case Plateau[NB_LIGNE][NB_COLONNE];

typedef struct {
    Case joueur;
    Plateau plateau;
    unsigned nb_jetons;
} puissance_quatre_st;

typedef struct {
    void *data;
    void (*affichage)(void * data, puissance_quatre_st *game);
    void (*affichage_fin_partie)(void * data, puissance_quatre_st *game);
    int (*get_prochain_coup)(void *data, unsigned *colonne);
} userInterface;

typedef struct {
    void 
} modeJeu;

#endif
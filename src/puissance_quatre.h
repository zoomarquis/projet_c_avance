#ifndef PUISSANCE_QUATRE_H
#define PUISSANCE_QUATRE_H

#define NB_LIGNE 6
#define NB_COLONNE 7

typedef enum
{
    VIDE,
    J1,
    J2
} Type;

typedef Type Plateau[NB_LIGNE][NB_COLONNE];

typedef struct joueur_ Joueur;

typedef struct {
    //Type joueur; // joueur courant, si = VIDE : fin partie : perdu
    Joueur * j1;
    Joueur * j2;
    Joueur * courant;
    Plateau plateau;
    unsigned nb_jetons;
} Puissance4;

struct joueur_{
    void * data; // arbre decision pour ia
    Type c;
    unsigned (*jouer)(void * data, Puissance4 game);
};

typedef struct {
    void *data;
    void (*affichage)(void * data, Puissance4 *game);
    void (*affichage_fin_partie)(void * data, Puissance4 *game);
    void (*get_prochain_coup)(void *data, Puissance4 * game, unsigned *colonne);
} userInterface;

int testColonneDisponible(Plateau plateau, unsigned c);
void playGame(Puissance4 *game, userInterface ui);

#endif
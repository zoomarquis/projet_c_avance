/**
 * @file puissance_quatre.h
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Définition des types et fonctions de base du jeu puissance quatre.
 * @version 0.1
 * @date 2022-12-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PUISSANCE_QUATRE_H
#define PUISSANCE_QUATRE_H

#include <stdbool.h>

/**
 * @def NB_LIGNE
 * @brief nombre de ligne dans le plateau
 */
/**
 * @def NB_COLONNE
 * @brief nombre de colonne dans le plateau
 */
#define NB_LIGNE 6
#define NB_COLONNE 7

/**
 * @enum Type
 * @brief Représente les cases du plateau
 * @typedef Type
 */
typedef enum {
  VIDE, //!< Représente une case vide
  J1,   //!< Représente une case qui est occupée par J1
  J2    //!< Représente une case qui est occupée par J2
} Type;

/**
 * \typedef Plateau
 * @brief Un tableau à 2 dimensions de Type
 */
typedef Type Plateau[NB_LIGNE][NB_COLONNE];

/**
 * @typedef Joueur
 * @brief Renommer joueur_
 */
typedef struct joueur_ Joueur;

typedef struct {
  Joueur *j1;
  Joueur *j2;
  Joueur *courant; // courant à vide => perdu
  Plateau plateau;
  bool fin; // booleen pour la fonction d'affichage
  unsigned nb_jetons;
} Puissance4;

struct joueur_ {
  Type type;
  unsigned char profondeur; // pour l'IA
  unsigned (*play)(Puissance4 *);
};

typedef struct {
  void *data;
  void (*affichage)(void *data, Puissance4 *game);
  void (*getProchainCoup)(void *data, Puissance4 *game, unsigned *colonne,
                          unsigned *ligne);
} userInterface;

bool testEnd(Puissance4 *, unsigned, unsigned);
int testColonne(Plateau, int);
void modifJeton(Puissance4 *, unsigned, unsigned, Type);
void changerJoueur(Puissance4 *);
void launchGame(Puissance4 *, userInterface);

#endif
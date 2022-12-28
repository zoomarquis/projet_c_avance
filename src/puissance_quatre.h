/**
 * @file puissance_quatre.h
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Définition des types et fonctions de base du jeu puissance quatre.
 * @version 1.0
 * @date 2022-12-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PUISSANCE_QUATRE_H
/**
 * @def PUISSANCE_QUATRE_H
 * @brief la garde
 */
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
/**
 * @def NB_ALIGNE
 * @brief nombre de pions alignés pour gagner
*/
#define NB_LIGNE 6
#define NB_COLONNE 7
#define NB_ALIGNE 4

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

/**
 * @struct p4_
 * @brief Représente un jeu du puissance 4.
 * @typedef Puissance4 p4_
 */
typedef struct p4_ {
  Joueur *j1;      //<! Pointeur sur J1
  Joueur *j2;      //<! Pointeur sur J2
  Joueur *courant; //<! Pointeur sur le joueur courant, si NULL : partie terminée et égalité
  Plateau plateau; //<! Le plateau (tableau de cases)
  unsigned ligne, colonne; //!< dernier coup joué
  unsigned nb_jetons; //<! Nombre de jetons sur le plateau
} Puissance4;

/**
 * @struct joueur_
 * @brief Représente un joueur.
 */
struct joueur_ {
  Type type; //<! Pour savoir si le joueur est un J1 ou un J2
  unsigned char profondeur; //<! Si le joueur est une IA, correspond à son niveau de difficulté
  unsigned (*play)(Puissance4 *); //<! Pointeur de fonction : jouer, récupérer le coup souhaité.
};

/**
 * @struct ui_
 * @brief Représente l'interface graphique du jeu.
 * @typedef userInterface ui_
 */
typedef struct ui_{
  void *data;
  void (*initAffichage)(void *data, Puissance4 *game);
  void (*affichage)(void *data, Puissance4 *game);
  void (*getProchainCoup)(void *data, Puissance4 *game);
  void (*endAffichage)(void * data, Puissance4 *game);
} userInterface;

bool testEnd(Puissance4 *, unsigned, unsigned);
int testColonne(Plateau, int);
void modifJeton(Puissance4 *, unsigned, unsigned, Type);
void changerJoueur(Puissance4 *);
void launchGame(Puissance4 *, userInterface);

#endif
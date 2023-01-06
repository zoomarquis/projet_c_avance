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
 * @enum type_
 * @brief Représente les cases du plateau
 * @typedef Type
 * @brief Renommer type_.
 */
typedef enum type_ {
  VIDE, //!< Représente une case vide
  J1,   //!< Représente une case qui est occupée par J1
  J2    //!< Représente une case qui est occupée par J2
} Type;

/**
 * @typedef Plateau
 * @brief Un tableau à 2 dimensions de Type.
 */
typedef Type Plateau[NB_LIGNE][NB_COLONNE];

/**
 * @typedef Joueur
 * @brief Renommer un joueur_.
 */
typedef struct joueur_ Joueur;

/**
 * @struct p4_
 * @brief Représente un jeu du puissance 4.
 * @typedef Puissance4
 * @brief Renommer p4_.
 */
typedef struct p4_ {
  Joueur *j1;         ///< Pointeur sur J1
  Joueur *j2;         //!< Pointeur sur J2
  Joueur *courant;    /*!< Pointeur sur le joueur courant, si NULL : partie
                         terminée et égalité */
  Plateau plateau;    //!< Le plateau (tableau de cases)
  unsigned ligne;     //!< coordonnée du dernier coup : sa ligne
  unsigned colonne;   //!< coordonnée du dernier coup : sa colonne
  unsigned nb_jetons; //!< Nombre de jetons sur le plateau
  bool rageQuit;      //!< Booléen en cas de rage quit en mode graphique
} Puissance4;

/**
 * @struct joueur_
 * @brief Représente un joueur.
 */
struct joueur_ {
  Type type;                /*!< Pour savoir si le joueur est un J1 ou un J2 */
  unsigned char profondeur; /*!<  Si le joueur est une IA, correspond à son
                                niveau de difficulté */
  unsigned (*play)(Puissance4 *); /*!< Pointeur de fonction : jouer, récupérer
                                      le coup souhaité. */
};

/**
 * @struct ui_
 * @brief Représente l'interface graphique du jeu.
 * @typedef userInterface
 * @brief Renommer ui_.
 */
typedef struct ui_ {
  void *data; /*!<  Void * pour faire de la généricité */
  void (*initAffichage)(void *data,
                        Puissance4 *game); /*!<  Pointeur de fonction : affiche
                                              le plateau en début de partie */
  void (*affichage)(void *data,
                    Puissance4 *game); /*!<  Pointeur de fonction : met à jour
                                          l'affichage du plateau */
  void (*getProchainCoup)(
      Puissance4 *game); /*!< Pointeur de fonction : récupère le prochain coup
                            du joueur courant */
  bool (*endAffichage)(
      void *data,
      Puissance4 *game); /*!<  Pointeur de fonction : affiche le jeu une fois la
                            partie terminée, propose de rejouer */
  void (*destroy)(
      void *data); /*!< Pointeur de fonction : détruit l'interface */
} userInterface;

bool testAlign(Plateau, unsigned, unsigned, int, int);
bool testEnd(Puissance4 *, unsigned, unsigned);
void modifJeton(Puissance4 *, unsigned, unsigned, Type);
int testColonne(Plateau, unsigned);
void changerJoueur(Puissance4 *game);
void initGame(Puissance4 *);
void prochainCoup(Puissance4 *);
void launchGame(Puissance4 *, userInterface *);
Puissance4 *initPuissance4();
void clean(Puissance4 *, userInterface *);

#endif
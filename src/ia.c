/**
 * @file ia.c
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Ensemble des fonctions utiles à l'IA : création, play (jouer le
 * prochain coup) et toutes les fonctions nécessaires au calcul du prochain
 * coup.
 * @version 1.0
 * @date 2022-12-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ia.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @def MAX
 * @brief la valeur maximale d'une évaluation = partie gagnée
 */
#define MAX 10000

/**
 * @struct couple_
 * @brief Un couple indice de la colonne et sa valeur associée.
 * @typedef Couple
 * @brief Renommer un couple_.
 */
typedef struct couple_ {
  int indice; //!< indice de la colonne
  int valeur; //!< valeur associée à la colonne
} Couple;

/**
 * @brief Évalue une case. Heuristique : si la case appartient au joueur courant
 * alors elle est évaluée à 2, si elle est vide à 1 et 0 sinon. 0 si la case
 * n'est pas dans le plateau.
 *
 * @param game le jeu
 * @param ligne le numéro de la ligne de la case à évaluer
 * @param colonne le numéro de la colonne à évaluer
 * @return unsigned
 */
static unsigned valeurCase(Puissance4 game, unsigned ligne, unsigned colonne) {
  if (ligne < 0 || ligne >= NB_LIGNE || colonne < 0 || colonne >= NB_COLONNE)
    return 0;
  if (game.plateau[ligne][colonne] == VIDE)
    return 1;
  else if (game.plateau[ligne][colonne] == game.courant->type)
    return 2;
  return 0;
}

/**
 * @brief Calcule le score d'une case en fonction de toutes les cases autour
 * (distance de 1) de celle-ci.
 *
 * @param game le jeu
 * @param ligne le numéro de ligne de la case
 * @param colonne le numéro de colonne de la case
 * @return unsigned le score de la case
 */
static unsigned autour(Puissance4 game, unsigned ligne, unsigned colonne) {
  assert(ligne >= 0 && ligne < NB_LIGNE);
  assert(colonne >= 0 && colonne < NB_COLONNE);
  unsigned som = 0;
  som += valeurCase(game, ligne - 1, colonne - 1);
  som += valeurCase(game, ligne - 1, colonne);
  som += valeurCase(game, ligne - 1, colonne + 1);
  som += valeurCase(game, ligne, colonne - 1);
  som += valeurCase(game, ligne, colonne + 1);
  som += valeurCase(game, ligne + 1, colonne - 1);
  som += valeurCase(game, ligne + 1, colonne);
  som += valeurCase(game, ligne + 1, colonne + 1);
  return som;
}

/**
 * @brief Evaluer le score du Joueur courant
 *
 * @param game le jeu
 * @return unsigned le score du joueur
 */
static unsigned scoreJoueur(Puissance4 game) {
  assert(game.courant->type != VIDE);
  unsigned som = 0;
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      if (game.plateau[i][j] == game.courant->type) {
        som += autour(game, i, j);
      }
    }
  }
  return som;
}

/**
 * @brief Fonction d'évaluation du plateau. (pour le joueur qui n'est pas le
 * courant car on change de joueur avant l'appel récursif dans minimax mais on
 * évalue au début de cette même fonction pour le joueur qui vient de jouer)
 *
 * @param game le jeu
 * @return int la valeur associée pour un joueur au plateau
 */
static int evaluation(Puissance4 *game) {
  assert(game);
  int val;
  changerJoueur(game);
  val = scoreJoueur(*game);
  changerJoueur(game);
  val -= scoreJoueur(*game);
  return val;
}

/**
 * @brief Fonction récursive pour déterminer le meilleur coup.
 *
 * @param game le jeu
 * @param profondeur la profondeur pour la récursivité
 * @param colonne la dernière colonne jouée
 * @return Couple un couple contenant le meilleur coup : l'indice de la colonne
 * du meilleur coup et la valeur du meilleur coup
 */
static Couple minimax(Puissance4 *game, unsigned profondeur, int colonne) {
  assert(game);
  assert(colonne == -1 || (colonne >= 0 && colonne < NB_COLONNE));
  Couple res;
  int ligne;
  int bestColonne = -1;
  int bestValeur = MAX + 1;
  if (colonne != -1) { // premier appel : pas encore de coup joué
    ligne = testColonne(game->plateau, colonne);
    ligne++; // le coup qu'on vient de jouer
    Joueur *tmp = game->courant;
    if (testEnd(game, ligne, colonne)) {
      if (!game->courant) { // égalité
        game->courant = tmp;
        return (Couple){colonne, 0};
      }
      return (Couple){colonne, MAX};
    }
  }

  if (profondeur == 0) { // fin de la recherche en profondeur
    return (Couple){colonne, evaluation(game)};
  }

  for (int i = 0; i < NB_COLONNE; i++) {
    ligne = testColonne(game->plateau, i);
    if (ligne != -1) { // on peut jouer dans cette colonne
      modifJeton(game, ligne, i, game->courant->type); // do
      changerJoueur(game);
      res = minimax(game, profondeur - 1, i);
      int valeur_courante = -res.valeur;
      if (valeur_courante < bestValeur) {
        bestValeur = valeur_courante;
        bestColonne = i;
      }
      changerJoueur(game);
      modifJeton(game, ligne, i, VIDE); // undo
    }
  }
  return (Couple){bestColonne, bestValeur};
}

/**
 * @brief Sélectionne la colonne à jouer par l'IA.
 *
 * @param game le jeu
 * @return unsigned la colonne où l'IA place un pion
 */
static unsigned playIA(Puissance4 *game) {
  assert(game);
  Couple res = minimax(game, game->courant->profondeur, -1);
  assert(res.indice >= 0 && res.indice < NB_COLONNE);
  return (unsigned)res.indice;
}

/**
 * @brief Crée un joueur IA.
 *
 * @param t le type du Joueur
 * @param niveau le niveau de difficulté
 * @return Joueur* un pointeur sur le Joueur créé
 */
Joueur *makeIA(Type t, char niveau) {
  assert(t != VIDE);
  assert(niveau == '1' || niveau == '2' || niveau == '3');
  Joueur *j = malloc(sizeof(Joueur));
  if (!j) {
    perror("Problème d'allocation dans makeIA.");
    return NULL;
  }
  j->type = t;
  niveau = niveau - '0';
  switch (niveau) {
  case 1:
    niveau = 1;
    break;
  case 2:
    niveau = 3;
    break;
  case 3:
    niveau = 5;
    break;
  }
  j->profondeur = niveau;
  j->play = &playIA;
  return j;
}
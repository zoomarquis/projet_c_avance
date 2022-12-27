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
#include <stdbool.h>
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
 * @typedef Couple couple_
 */
typedef struct couple_ {
  int indice; //!< indice de la colonne
  int valeur; //!< valeur associée à la colonne
} Couple;

static unsigned valeurCase(Puissance4 game, unsigned ligne, unsigned colonne) {
  if (ligne < 0 || ligne >= NB_LIGNE || colonne < 0 || colonne >= NB_COLONNE)
    return 0;
  if (game.plateau[ligne][colonne] == VIDE)
    return 1;
  else if (game.plateau[ligne][colonne] == game.courant->type)
    return 2;
  return 0;
}

static unsigned autour(Puissance4 game, unsigned ligne, unsigned colonne) {
  // ?
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

static unsigned scoreJoueur(Puissance4 game, Joueur *player) {
  // assert
  unsigned som = 0;
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      if (game.plateau[i][j] == player->type) {
        som += autour(game, i, j);
      }
    }
  }
  return som;
}

static int evaluation(Puissance4 *game) {
  int val;
  changerJoueur(game);
  val = scoreJoueur(*game, game->courant);
  changerJoueur(game);
  val -= scoreJoueur(*game, game->courant);
  return val;
}

static Couple minimax(Puissance4 *game, unsigned profondeur, int colonne) {
  Couple res;
  int ligne;
  int bestColonne = -1;
  int bestValeur = MAX + 1;
  if (colonne != -1) { // premier appel : pas encore joué
    ligne = testColonne(game->plateau, colonne);
    ligne++; // le coup qu'on vient de jouer
    Joueur *tmp = game->courant;
    if (testEnd(game, ligne, colonne)) {
      if (!game->courant) { // egalite
        game->courant = tmp;
        return (Couple){colonne, 0};
      }
      return (Couple){colonne, MAX};
    }
  }

  if (profondeur == 0) {
    return (Couple){colonne, evaluation(game)};
  }

  for (int i = 0; i < NB_COLONNE; i++) {
    ligne = testColonne(game->plateau, i);
    if (ligne != -1) {
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

// retourne la colonne à jouer
static unsigned playIA(Puissance4 *game) {
  Couple res = minimax(game, game->courant->profondeur, -1);
  assert(res.indice >= 0 && res.indice < NB_COLONNE);
  return (unsigned)res.indice;
}

Joueur *makeIA(Type t, char niveau) {
  assert(niveau == '1' || niveau == '2' || niveau == '3');
  Joueur *j = malloc(sizeof(Joueur));
  if (!j) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  j->type = t;
  niveau = niveau - '0';
  switch (niveau) {
  case 1:
    niveau = 2;
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
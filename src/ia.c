#include "ia.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX 1000

typedef struct {
  unsigned valeur, profondeur;
} Couple;

static unsigned autour(Puissance4 game, unsigned ligne, unsigned colonne) {
  // ?

  unsigned som = 0;
  if (ligne - 1 >= 0 && colonne - 1 >= 0 && (game.plateau[ligne][colonne]==VIDE || game.plateau[ligne][colonne]==game.courant->type) {
  }
}

static unsigned evaluation(Puissance4 game) {
  // assert pas fin de partie ?
  // assert pas ?

  unsigned som = 0;
  // pour chaque case du tab au joueur : évaluer
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      if (game.plateau[i][j] == game.courant->type) {
        som += autour(game, i, j);
      }
    }
  }
}

int evaluation(Puissance4 *game, unsigned colonne) {
  assert(game->courant);

  int ret = 1;
  Joueur *tmp = game->courant;
  int ligne = testColonne(game->plateau, colonne);
  if (ligne == -1)
    return -INF;

  modifJeton(game, ligne, colonne, game->courant->c); // do

  bool res = testEnd(game, ligne, colonne);
  if (!game->courant) { // quelle valeur si perdu?
    res = false;
    game->courant = tmp;
    // ret =
  } else if (res)
    ret = NB_LIGNE * NB_COLONNE; // coup gagnant
  else {
    ret += evalAlign(game->plateau, ligne, colonne, 0, 1);
    ret += evalAlign(game->plateau, ligne, colonne, 1, 0);
    ret += evalAlign(game->plateau, ligne, colonne, 1, 1);
    ret += evalAlign(game->plateau, ligne, colonne, 1, -1);
  }

  modifJeton(game, ligne, colonne, VIDE); // undo
  return ret;
}

unsigned playIA(Puissance4 *game, unsigned profondeur) {
  int res, max;    // = ?? minimum
  unsigned indice; // = ??
  for (int i = 0; i < NB_COLONNE; i++) {
    // do
    // changer de joueur ?
    res = calcul(game, profondeur - 1);
    if (max < res) {
      max = res;
      indice = i;
    }
    // undo
  }
  return indice;
}

int calcul(Puissance4 *game, unsigned profondeur) {
  int res, max; // = ?? minimum
  // if (profondeur==0 || testEnd(game,? ligne, ? colonne))
  // return eval(?)
  for (int i = 0; i < NB_COLONNE; i++) {
    // do
    // changer de joueur ?
    res = -calcul(game, profondeur - 1);
    if (max < res) {
      max = res;
    }
    // undo
  }
  return max;
}
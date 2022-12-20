#include "puissance_quatre.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool testAlignement(Plateau plateau, unsigned ligne, unsigned colonne,
                           int deplaL, int deplaC) {
  assert(ligne >= 0 && ligne < NB_LIGNE);
  assert(colonne >= 0 && colonne < NB_COLONNE);
  assert(deplaL == 1 || deplaL == -1 || deplaL == 0);
  assert(deplaC == 1 || deplaC == -1 || deplaC == 0);
  Type valeur = plateau[ligne][colonne];
  assert(valeur == J1 || valeur == J2);
  unsigned nb_aligne = 1, l = ligne, c = colonne;
  while (l >= 0 && l < NB_LIGNE && c >= 0 && c < NB_COLONNE &&
         plateau[l + deplaL][c + deplaC]) {
    l += deplaL;
    c += deplaC;
    nb_aligne++;
  }
  if (nb_aligne >= 4)
    return true;
  l = ligne;
  c = colonne;
  while (l >= 0 && l < NB_LIGNE && c >= 0 && c < NB_COLONNE &&
         plateau[l - deplaL][c - deplaC]) {
    l -= deplaL;
    c -= deplaC;
    nb_aligne++;
  }
  return (nb_aligne >= 4);
}

static bool testFinPartie(Puissance4 *game, unsigned l, unsigned c) {
  if (game->nb_jetons == (NB_COLONNE * NB_LIGNE)) {
    game->courant = NULL;
    return true;
  }
  if (testAlignement(game->plateau, l, c, 0, 1)      // horizontal
      || testAlignement(game->plateau, l, c, 1, 0)   // vertical
      || testAlignement(game->plateau, l, c, 1, 1)   // diagonal
      || testAlignement(game->plateau, l, c, 1, -1)) // diagonal /
    return true;
  return false;
}

int testColonneDisponible(Plateau plateau, unsigned c) {
  assert(c >= 0 && c < NB_COLONNE);
  for (int i = NB_LIGNE - 1; i >= 0; i--) {
    if (plateau[i][c] == VIDE)
      return i;
  }
  return -1;
}

static void ajoutJeton(Puissance4 *game, unsigned ligne, unsigned colonne) {
  assert(game->courant != NULL);
  assert(ligne >= 0 && ligne < NB_LIGNE);
  assert(colonne >= 0 && colonne < NB_COLONNE);
  game->plateau[ligne][colonne] = game->courant->c;
  game->nb_jetons++;
}

void playGame(Puissance4 *game, userInterface ui) {
  unsigned colonne, ligne;
  do {
    if (game->courant == game->j2)
      game->courant = game->j1;
    else if (game->courant == game->j1)
      game->courant = game->j2;
    else {
      perror("Problème inattendu.");
      exit(EXIT_FAILURE);
    }
    ui.affichage(ui.data, game);
    ui.get_prochain_coup(ui.data, game, &colonne, &ligne);
    ajoutJeton(game, ligne, colonne);
  } while (!testFinPartie(game, ligne, colonne));
  ui.affichage_fin_partie(ui.data, game);
}

// endgame retourne VIDE si pas fini, sinon retourne le gagnant
// commande pour tout arreter ?
// affichage fin partie
// affichage
// get prochain coup
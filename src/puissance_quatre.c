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
  Case valeur = plateau[ligne][colonne];
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

static bool testFinPartie(puissance_quatre_st game, unsigned l, unsigned c) {
  if (game.nb_jetons == (NB_COLONNE * NB_LIGNE)) {
    game.joueur = VIDE;
    return true;
  }
  if (testAlignement(game.plateau, l, c, 0, 1)      // horizontal
      || testAlignement(game.plateau, l, c, 1, 0)   // vertical
      || testAlignement(game.plateau, l, c, 1, 1)   // diagonal
      || testAlignement(game.plateau, l, c, 1, -1)) // diagonal /
    return true;
  return false;
}

/** retourne -1 si la colonne n'est pas disponible
 * sinon retourne l'indice de la première case vide
 */
static int testColonneDisponible(Plateau plateau, unsigned c) {
  assert(c >= 0 && c < NB_COLONNE);
  for (int i = NB_LIGNE - 1; i >= 0; i--) {
    if (plateau[i][c] == VIDE)
      return i;
  }
  return -1;
}

static void ajoutJeton(puissance_quatre_st *game, unsigned ligne,
                       unsigned colonne) {
  assert(game->joueur == J1 || game->joueur == J2);
  assert(ligne >= 0 && ligne < NB_LIGNE);
  assert(colonne >= 0 && colonne < NB_COLONNE);
  game->plateau[ligne][colonne] = game->joueur;
  game->nb_jetons++;
}

puissance_quatre_st *initGame() {
  puissance_quatre_st *game = malloc(sizeof(puissance_quatre_st));
  if (!game) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      game->plateau[i][j] = VIDE;
    }
  }
  return game;
}

void playGame(puissance_quatre_st *game, userInterface ui) {
  unsigned colonne, ligne;
  do {
    changer_joueur(game);
    ui.affichage(ui.data, game);
    ui.get_prochain_coup(ui.data, &colonne);
    while ((ligne = testColonneDisponible(game->plateau, colonne)) != -1) {
      // message erreur ??
    }
    ajoutJeton(game, ligne, colonne);
  } while (!testFinPartie(*game, ligne, colonne));
  ui.affichage_fin_partie(ui.data, game);
}

// endgame retourne VIDE si pas fini, sinon retourne le gagnant
// commande pour tout arreter ?
// affichage fin partie
// affichage
// get prochain coup
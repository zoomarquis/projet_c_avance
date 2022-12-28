/**
 * @file puissance_quatre.c
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Ensemble de fonctions de base pour le jeu puissance quatre :
 * lancer le jeu, tester si la partie est terminée, ajouter / enlever un jeton.
 * @version 1.0
 * @date 2022-12-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "puissance_quatre.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool testAlign(Plateau plateau, unsigned ligne, unsigned colonne,
                      int deplaL, int deplaC) {
  assert(ligne >= 0 && ligne < NB_LIGNE);
  assert(colonne >= 0 && colonne < NB_COLONNE);
  assert(deplaL == 1 || deplaL == -1 || deplaL == 0);
  assert(deplaC == 1 || deplaC == -1 || deplaC == 0);
  Type valeur = plateau[ligne][colonne];
  assert(valeur == J1 || valeur == J2);
  unsigned nb_aligne = 1;
  int l = ligne, c = colonne;
  while (nb_aligne < NB_ALIGNE && l + deplaL >= 0 && l + deplaL < NB_LIGNE &&
         c + deplaC >= 0 && c + deplaC < NB_COLONNE &&
         plateau[l + deplaL][c + deplaC] == valeur) {
    l += deplaL;
    c += deplaC;
    nb_aligne++;
  }
  if (nb_aligne >= NB_ALIGNE)
    return true;
  l = ligne;
  c = colonne;
  while (nb_aligne < NB_ALIGNE && l - deplaL >= 0 && l - deplaL < NB_LIGNE &&
         c - deplaC >= 0 && c - deplaC < NB_COLONNE &&
         plateau[l - deplaL][c - deplaC] == valeur) {
    l -= deplaL;
    c -= deplaC;
    nb_aligne++;
  }
  return (nb_aligne >= NB_ALIGNE);
}

bool testEnd(Puissance4 *game, unsigned l, unsigned c) {
  if (testAlign(game->plateau, l, c, 0, 1)      // horizontal
      || testAlign(game->plateau, l, c, 1, 0)   // vertical
      || testAlign(game->plateau, l, c, 1, 1)   // diagonal
      || testAlign(game->plateau, l, c, 1, -1)) // diagonal /
    return true;
  if (game->nb_jetons == (NB_COLONNE * NB_LIGNE)) {
    game->courant = NULL;
    return true;
  } // égalité
  return false;
}

void modifJeton(Puissance4 *game, unsigned ligne, unsigned colonne, Type type) {
  assert(game->courant != NULL);
  assert(ligne >= 0 && ligne < NB_LIGNE);
  assert(colonne >= 0 && colonne < NB_COLONNE);
  if (type != VIDE) {
    assert(game->plateau[ligne][colonne] == VIDE);
    game->nb_jetons++;
  } else {
    assert(game->plateau[ligne][colonne] != VIDE);
    game->nb_jetons--;
  }
  game->plateau[ligne][colonne] = type;
}

int testColonne(Plateau plateau, int c) {
  assert(c >= 0 && c < NB_COLONNE);
  for (int i = NB_LIGNE - 1; i >= 0; i--) {
    if (plateau[i][c] == VIDE)
      return i;
  }
  return -1;
}

void changerJoueur(Puissance4 *game) {
  assert(game->courant);
  if (game->courant == game->j2)
    game->courant = game->j1;
  else if (game->courant == game->j1)
    game->courant = game->j2;
  else {
    perror("Problème inattendu.");
    exit(EXIT_FAILURE);
  }
}

static void reInitGame(Puissance4 *game) {
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      game->plateau[i][j] = VIDE;
    }
  }
  game->nb_jetons = 0;
}

void launchGame(Puissance4 *game, userInterface *ui) {
  // assert !!
  bool rejouer;
jouer:
  game->courant = game->j2;
  ui->initAffichage(ui->data, game);
  do {
    changerJoueur(game);
    if (game->rageQuit)
      return;
    ui->getProchainCoup(game); // graphique ? -> return ;
    if (game->rageQuit)
      return;
    modifJeton(game, game->ligne, game->colonne, game->courant->type);
    ui->affichage(ui->data, game);
  } while (!(testEnd(game, game->ligne, game->colonne)));
  if (game->rageQuit) // celui là nécessaire ?
    return;
  rejouer = ui->endAffichage(ui->data, game);
  if (game->rageQuit)
    return;
  if (rejouer) {
    reInitGame(game);
    goto jouer;
  }
}
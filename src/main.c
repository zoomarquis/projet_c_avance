/**
 * @file main.c
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Le fichier main du jeu puissance quatre.
 * @version 0.1
 * @date 2022-12-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "choixModes.h"
#include "console.h"
#include "graphique.h"
#include "ia.h"
#include "puissance_quatre.h"

#include <stdio.h>
#include <stdlib.h>

Puissance4 *initGame() {
  Puissance4 *game = malloc(sizeof(Puissance4));
  if (!game) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      game->plateau[i][j] = VIDE;
    }
  } // init plateau
  game->fin = false;
  game->nb_jetons = 0;
  return game;
}

void cleanGame(Puissance4 *game, userInterface *ui) {
  free(game->j1);
  free(game->j2);
  free(ui);
  free(game);
}

int main() {
  char interface, mode, niveau, niveau2;
  printf("PUISSANCE QUATRE\n");

  choix_interface(&interface);
  choix_mode(&mode);
  if (mode == 'i' || mode == 'a') {
    choix_niveau(&niveau);
  } else if (mode == 's') {
    choix_niveaux(&niveau, &niveau2);
  }

  userInterface *ui;
  Puissance4 *game = initGame();

  if (interface == 'c') {
    ui = makeConsole();
    if (mode == 'h') {
      game->j1 = makeHumainConsole(J1);
      game->j2 = makeHumainConsole(J2);
    } else if (mode == 'a') {
      game->j1 = makeHumainConsole(J1);
      game->j2 = makeIA(J2, niveau);
    } else if (mode == 'i') {
      game->j1 = makeIA(J1, niveau);
      game->j2 = makeHumainConsole(J2);
    } else if (mode == 's') {
      game->j1 = makeIA(J1, niveau);
      game->j2 = makeIA(J2, niveau2);
    } else {
      perror("Erreur inopinée !");
      exit(EXIT_FAILURE);
    }
    game->courant = game->j2; // switch au début de partie sur j1
  } else if (interface == 'g') {
    ui = makeGraphique();
  } else {
    perror("Erreur inopinée !");
    exit(EXIT_FAILURE);
  }

  launchGame(game, *ui);

  cleanGame(game, ui);

  return EXIT_SUCCESS;
}
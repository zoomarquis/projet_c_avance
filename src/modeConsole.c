#include "puissance_quatre.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void printLigneIntermediaire() {
  printf("\n");
  for (int c = 0; c < NB_COLONNE; c++)
    printf("+---");
  printf("+");
  printf("\n");
}

static void printCase(Type v) {
  switch (v) {
  case VIDE:
    printf("   |");
    break;
  case J1:
    printf(" X |");
    break;
  case J2:
    printf(" O |");
    break;
  }
}

static void printPlateau(void *data, Puissance4 *game) {
  unsigned c, l;
  for (c = 0; c < NB_COLONNE; c++)
    printf("  %u ", c + 1);
  printLigneIntermediaire();

  for (l = 0; l < NB_LIGNE; l++) {
    printf("|");
    for (c = 0; c < NB_COLONNE; c++)
      printCase(game->plateau[l][c]);
    printLigneIntermediaire();
  }

  for (c = 0; c < NB_COLONNE; c++)
    printf("  %u ", c + 1);
  printf("\n");

  if (game->fin) {
    if (!game->courant)
      printf("Perdu !\n");
    else
      (game->courant->c == J1) ? (printf("Joueur 1 a gagné !\n"))
                               : (printf("Joueur 2 a gagné !\n "));
  }
}

static unsigned jouerHumainConsole(void *data, Puissance4 game) {
  int coup;
  assert(game.courant);
  (game.courant->c == J1) ? (printf("Joueur 1 : ")) : (printf("Joueur 2 : "));
  scanf("%d", &coup);
  while (coup < 1 || coup > NB_COLONNE) {
    printf("Veuillez entrer un numéro de colonne valide, entre %d et %d.\n", 1,
           NB_COLONNE);
    scanf("%d", &coup);
  }
  return coup;
}
// que faire si pas ok ?
// vider buffer

static void prochainCoup(void *data, Puissance4 *game, unsigned *colonne,
                         unsigned *ligne) {
  assert(game->courant);
  unsigned coup = game->courant->jouer(NULL, *game);
  while (testColonneDisponible(game->plateau, coup - 1) == -1) {
    printf("Cette colonne est pleine, veuillez en choisir une autre.");
    coup = game->courant->jouer(NULL, *game);
  }
  *colonne = coup - 1;
  *ligne = testColonneDisponible(game->plateau, coup - 1);
  assert(*ligne != -1);
}

userInterface *makeConsole() {
  userInterface *ui = malloc(sizeof(userInterface));
  if (!ui) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  ui->affichage = printPlateau;
  ui->getProchainCoup = prochainCoup;
  return ui;
}

Joueur *makeHumainConsole(Type c, Puissance4 *game) {
  Joueur *j = malloc(sizeof(Joueur));
  if (!j) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  j->c = c;
  j->jouer = &jouerHumainConsole;
  return j;
}
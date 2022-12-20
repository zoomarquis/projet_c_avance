#include "puissance_quatre.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void printLigneIntermediaire()
{
  printf("\n");
  for (int c = 0; c < NB_COLONNE; c++)
    printf("+---");
  printf("+");
  printf("\n");
}

static void printCase(Type v)
{
  switch (v)
  {
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

static void printPlateau(void *data, Puissance4 *game)
{
  unsigned c, l;
  for (c = 0; c < NB_COLONNE; c++)
    printf("  %u ", c + 1);
  printLigneIntermediaire();

  for (l = 0; l < NB_LIGNE; l++)
  {
    printf("|");
    for (c = 0; c < NB_COLONNE; c++)
      printCase(game->plateau[l][c]);
    printf("  %u ", l + 1);
    printLigneIntermediaire();
  }
}

int jouerHumainConsole(Puissance4 game)
{
  int coup;
  assert(game.courant);
  (game.courant->c == J1) ? (printf("Joueur 1 : ")) : (printf("Joueur 2 : "));
  scanf("%d", &coup);
  // que faire si pas ok ?
  // vider buffer
  return coup;
}

void prochain_coup(void *data, Puissance4 *game, unsigned *colonne,
                   unsigned *ligne)
{
  assert(game->courant);
  int coup = game->courant->jouer(NULL, *game);
  while (coup < 0 || coup >= NB_COLONNE ||
         testColonneDisponible(game->plateau, coup) != -1)
  {
    printf("Veuillez entrer un numéro de colonne valide, entre %d et %d.\n", 1,
           NB_COLONNE);
    coup = game->courant->jouer(NULL, *game);
  }
}

userInterface *makeConsole()
{
  userInterface *ui = malloc(sizeof(userInterface));
  if (!ui)
  {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  ui->affichage = printPlateau;
  // ui->affichage_fin_partie = printFinPartie;
  // ui->get_prochain_coup =
  return ui;
}
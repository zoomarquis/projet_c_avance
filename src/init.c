#include "mode_console.h"
#include "puissance_quatre.h"
#include <stdio.h>
#include <stdlib.h>

Puissance4 *initGame()
{
  Puissance4 *game = malloc(sizeof(Puissance4));
  if (!game)
  {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < NB_LIGNE; i++)
  {
    for (int j = 0; j < NB_COLONNE; j++)
    {
      game->plateau[i][j] = VIDE;
    }
  }
  return game;
}

Joueur *initHumainConsole(Type c, Puissance4 *game)
{
  Joueur *j = malloc(sizeof(Joueur));
  if (!j)
  {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  j->c = c;
  j->jouer = jouerHumainConsole;
  return j;
}

Joueur *initIAConsole(Type c, Puissance4 *game)
{
  // à implémenter
  // niveau ?
}

userInterface *init_console()
{
  return makeConsole();
}

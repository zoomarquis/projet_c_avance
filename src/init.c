#include "mode_console.h"
#include "puissance_quatre.h"
#include <stdio.h>
#include <stdlib.h>

Joueur *initHumainConsole(Type c, Puissance4 *game) {
  Joueur *j = malloc(sizeof(Joueur));
  if (!j) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  j->c = c;
  j->jouer = jouerHumainConsole(game);
  return j;
}

Joueur *initIAConsole(Type c, Puissance4 *game) {
  // à implémenter
  // niveau ?
}

userInterface *init_console() { makeConsole(); }

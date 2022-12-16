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

static void printCase(Case v) {
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

void printPlateau(Plateau plateau) {
  unsigned c, l;
  for (c = 0; c < NB_COLONNE; c++)
    printf("  %u ", c + 1);
  printLigneIntermediaire();

  for (l = 0; l < NB_LIGNE; l++) {
    printf("|");
    for (c = 0; c < NB_COLONNE; c++)
      printCase(plateau[l][c]);
    printf("  %u ", l + 1);
    printLigneIntermediaire();
  }
}

unsigned(Plateau p, Case joueur) {
  int coup;
  assert(joueur == J1 || joueur == J2);
  (joueur == J1) ? (printf("Joueur 1 : ")) : (printf("Joueur 2 : "));
  scanf("%hhd", &coup);
  while (coup < 0 || coup >= NB_COLONNE) {
    while ((coup = getchar()) != EOF && coup != '\n')
      ; // vider buffer
    printf("Veuillez entrer un numéro de colonne valide, entre %d et %d.\n", 1,
           NB_COLONNE);
    scanf("%hhd", &coup);
  };
  return coup;
}

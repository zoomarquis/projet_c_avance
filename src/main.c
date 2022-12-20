#include "init.c"
#include "puissance_quatre.h"
#include <stdio.h>
#include <stdlib.h>

void choix_interface(char *interface) {
  printf("Choisissez l'interface de jeu. Entrez :\n"
         "'g' pour jouer en mode interface graphique\n"
         "'c' pour jouer en mode console\n"
         "Votre choix : ");
  scanf("%hhd", interface);
  while (*interface != 'g' && *interface != 'c') {
    while ((*interface = getchar()) != EOF && *interface != '\n')
      ; // vider buffer
    printf("Entrée incorrecte. Veuillez réessayer : ");
    scanf("%hhd", interface);
  }
}

void choix_mode(char *mode) {
  printf("Choisissez un mode de jeu. Entrez :\n"
         "'h' pour jouer à deux\n"
         "'a' pour jouer contre une intelligence artificielle\n"
         "'i' pour qu'une intelligence artificielle joue contre vous\n"
         "'s' pour être spectateur d'une partie entre deux intelligences "
         "artificielles\nVotre choix : ");
  scanf("%hhd", mode);
  while (*mode != 'h' && *mode != 'a' && *mode != 'i' && *mode != 's') {
    while ((*mode = getchar()) != EOF && *mode != '\n')
      ; // vider buffer
    printf("Entrée incorrecte. Veuillez recommencer : ");
    scanf("%hhd", mode);
  }
}

void choix_niveau(char *niveau) {
  printf("Choisissez le niveau de l'intelligence artificielle. Entrez :\n"
         "'1' pour le mode facile\n"
         "'2' pour le mode moyen\n"
         "'3' pour le mode difficile\nVotre choix : ");
  scanf("%hhd", niveau);
  while (*niveau != '1' && *niveau != '2' && *niveau != '3') {
    while ((*niveau = getchar()) != EOF && *niveau != '\n')
      ; // vider buffer
    printf("Entrée incorrecte. Veuillez recommencer : ");
    scanf("%hhd", niveau);
  }
}

void choix_niveaux(char *niveau1, char *niveau2) {
  printf("Choisissez le niveau de la première intelligence artificielle. "
         "Entrez :\n"
         "'1' pour le mode facile\n"
         "'2' pour le mode moyen\n"
         "'3' pour le mode difficile\nVotre choix : ");
  scanf("%hhd", niveau1);
  while (*niveau1 != '1' && *niveau1 != '2' && *niveau1 != '3') {
    while ((*niveau1 = getchar()) != EOF && *niveau1 != '\n')
      ; // vider buffer
    printf("Entrée incorrecte. Veuillez recommencer : ");
    scanf("%hhd", niveau1);
  }

  printf("Choisissez le niveau de la seconde intelligence artificielle. "
         "Entrez :\n"
         "'1' pour le mode facile\n"
         "'2' pour le mode moyen\n"
         "'3' pour le mode difficile\nVotre choix : ");
  scanf("%hhd", niveau2);
  while (*niveau2 != '1' && *niveau2 != '2' && *niveau2 != '3') {
    while ((*niveau2 = getchar()) != EOF && *niveau2 != '\n')
      ; // vider buffer
    printf("Entrée incorrecte. Veuillez recommencer : ");
    scanf("%hhd", niveau1);
  }
}

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
  }
  return game;
}

int main() {
  char interface, mode, niveau, niveau2;
  printf("PUISSANCE QUATRE\n");

  // choix user :
  choix_interface(&interface);
  choix_mode(&mode);
  if (mode == 'i' || mode == 'a') {
    choix_niveau(&niveau);
  } else if (mode == 's') {
    choix_niveaux(&niveau, &niveau2);
  }

  Puissance4 *game = initGame();
  userInterface *ui;
  if (interface == 'c') {
    ui = init_console();
    if (mode == 'h') {
      game->j1 = initHumainConsole(J1, game);
      game->j2 = initHumainConsole(J2, game);
    } else if (mode == 'a') {
      game->j1 = initHumainConsole(J1, game);
      game->j2 = initIAConsole(J2, game);
    } else if (mode == 'i') {
      game->j1 = initIAConsole(J1, game);
      game->j2 = initHumainConsole(J2, game);
    } else if (mode == 's') {
      // wtf
    } else {
      perror("Erreur inopinée !");
      exit(EXIT_FAILURE);
    }
    game->courant = game->j2;
  } else if (interface == 'g') {
    // play(init_mode_console());
  } else {
    perror("Erreur inopinée !");
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}
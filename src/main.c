#include "modeConsole.h"
#include "puissance_quatre.h"
#include <stdio.h>
#include <stdlib.h>

void clear_buffer() {
  while (getchar() != '\n')
    ;
}

void choix_interface(char *interface) {
  printf("Choisissez l'interface de jeu. Entrez :\n"
         "'g' pour jouer en mode interface graphique\n"
         "'c' pour jouer en mode console\n"
         "Votre choix : ");
  scanf("%c", interface);
  clear_buffer();
  while (*interface != 'g' && *interface != 'c') {
    printf("Entrée incorrecte. Veuillez réessayer : ");
    scanf("%c", interface);
    clear_buffer();
  }
}

void choix_mode(char *mode) {
  printf("Choisissez un mode de jeu. Entrez :\n"
         "'h' pour jouer à deux\n"
         "'a' pour jouer contre une intelligence artificielle\n"
         "'i' pour qu'une intelligence artificielle joue contre vous\n"
         "'s' pour être spectateur d'une partie entre deux intelligences "
         "artificielles\nVotre choix : ");
  scanf("%c", mode);
  clear_buffer();
  while (*mode != 'h' && *mode != 'a' && *mode != 'i' && *mode != 's') {
    printf("Entrée incorrecte. Veuillez recommencer : ");
    scanf("%c", mode);
    clear_buffer();
  }
}

void choix_niveau(char *niveau) {
  printf("Choisissez le niveau de l'intelligence artificielle. Entrez :\n"
         "'1' pour le mode facile\n"
         "'2' pour le mode moyen\n"
         "'3' pour le mode difficile\nVotre choix : ");
  scanf("%c", niveau);
  clear_buffer();
  while (*niveau != '1' && *niveau != '2' && *niveau != '3') {
    printf("Entrée incorrecte. Veuillez recommencer : ");
    scanf("%c", niveau);
    clear_buffer();
  }
}

void choix_niveaux(char *niveau1, char *niveau2) {
  printf("Choisissez le niveau de la première intelligence artificielle. "
         "Entrez :\n"
         "'1' pour le mode facile\n"
         "'2' pour le mode moyen\n"
         "'3' pour le mode difficile\nVotre choix : ");
  scanf("%c", niveau1);
  clear_buffer();
  while (*niveau1 != '1' && *niveau1 != '2' && *niveau1 != '3') {
    printf("Entrée incorrecte. Veuillez recommencer : ");
    scanf("%c", niveau1);
    clear_buffer();
  }

  printf("Choisissez le niveau de la seconde intelligence artificielle. "
         "Entrez :\n"
         "'1' pour le mode facile\n"
         "'2' pour le mode moyen\n"
         "'3' pour le mode difficile\nVotre choix : ");
  scanf("%c", niveau2);
  clear_buffer();
  while (*niveau2 != '1' && *niveau2 != '2' && *niveau2 != '3') {
    printf("Entrée incorrecte. Veuillez recommencer : ");
    scanf("%c", niveau1);
    clear_buffer();
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
      game->j1 = makeHumainConsole(J1, game);
      game->j2 = makeHumainConsole(J2, game);
    } /*else if (mode == 'a') {
      game->j1 = initHumainConsole(J1, game);
      game->j2 = initIAConsole(J2, game);
    } else if (mode == 'i') {
      game->j1 = initIAConsole(J1, game);
      game->j2 = initHumainConsole(J2, game);
    } else if (mode == 's') {
      // wtf
    } */
    else {
      perror("Erreur inopinée !");
      exit(EXIT_FAILURE);
    }
    game->courant = game->j2; // switch au début de partie sur j1
  } else if (interface == 'g') {
    // play(init_mode_console());
  } else {
    perror("Erreur inopinée !");
    exit(EXIT_FAILURE);
  }
  playGame(game, *ui);

  cleanGame(game, ui);

  return EXIT_SUCCESS;
}
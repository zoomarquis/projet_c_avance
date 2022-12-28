/**
 * @file console.c
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Ensemble de fonctions pour le mode d'interface console du jeu : créer,
 * afficher, récupérer l'action
 * @version 1.0
 * @date 2022-12-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "console.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Affiche une ligne de pointillés pour séparer 2 rangées du plateau.
 */
static void printLigneIntermediaire() {
  printf("\n");
  for (int c = 0; c < NB_COLONNE; c++)
    printf("+---");
  printf("+");
  printf("\n");
}

/**
 * @brief Affiche une case du plateau.
 *
 * @param v le type de la case à afficher
 */
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

/**
 * @brief Affiche le plateau de jeu.
 *
 * @param data les données de l'interface, en mode console : inutile
 * @param game le jeu
 */
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
      printf("Égalité !\n");
    else
      (game->courant->type == J1) ? (printf("Joueur 1 a gagné !\n"))
                                  : (printf("Joueur 2 a gagné !\n "));
  }
}

/**
 * @brief Supprime ce qu'il y a dans le buffer de stdin.
 */
static void clearBuffer() {
  char c;
  while ((c = getchar()) != '\n' && (c != EOF))
    ;
}

/**
 * @brief Permet au joueur humain de jouer un pion.
 *
 * @param game le jeu
 * @return unsigned la colonne où le joueur place un pion
 */
static unsigned playHumainConsole(Puissance4 *game) {
  int coup;
  assert(game->courant);
  (game->courant->type == J1) ? (printf("Joueur 1 : "))
                              : (printf("Joueur 2 : "));
  scanf("%d", &coup);
  while (coup < 1 || coup > NB_COLONNE ||
         testColonne(game->plateau, coup - 1) == -1) {
    if (coup < 1 || coup > NB_COLONNE) {
      printf("Veuillez entrer un numéro de colonne valide, entre %d et %d.\n",
             1, NB_COLONNE);
    } else
      printf("Cette colonne est pleine, veuillez en choisir une autre.");
    clearBuffer();
    scanf("%d", &coup);
  }
  return coup - 1;
}

/**
 * @brief Récupère le prochain coup à jouer, sans tenir compte du type du joueur
 * (humain ou IA).
 *
 * @param data les données de l'interface, en mode console : inutile
 * @param game le jeu
 * @param colonne [out] la colonne du prochain coup
 * @param ligne [out] la ligne du prochain coup
 */
static void prochainCoup(void *data, Puissance4 *game, unsigned *colonne,
                         unsigned *ligne) {
  assert(game->courant);
  unsigned coup = game->courant->play(game);
  *colonne = coup;
  *ligne = testColonne(game->plateau, coup);
  assert(*ligne != -1);
  printf("\e[1;1H\e[2J");
}

/**
 * @brief Crée une interface en mode console.
 *
 * @return userInterface* un pointeur sur l'inferface crée
 */
userInterface *makeConsole() {
  userInterface *ui = malloc(sizeof(userInterface));
  if (!ui) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  ui->affichage = printPlateau;
  ui->getProchainCoup = prochainCoup;
  printf("\e[1;1H\e[2J");
  return ui;
}

/**
 * @brief Crée un joueur humain en mode console.
 *
 * @param t le type du Joueur
 * @return Joueur* un pointeur sur le Joueur créé
 */
Joueur *makeHumainConsole(Type t) {
  Joueur *j = malloc(sizeof(Joueur));
  if (!j) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  j->type = t;
  j->play = &playHumainConsole;
  return j;
}
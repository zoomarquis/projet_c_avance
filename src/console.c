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
#include <stdbool.h>
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
  assert(game);
  printf("\e[1;1H\e[2J");
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
  assert(game);
  assert(game->courant);
  (game->courant->type == J1) ? (printf("Joueur 1 (X): "))
                              : (printf("Joueur 2 (O): "));
  scanf("%d", &coup); // transfo en getchar ?
  while (coup < 1 || coup > NB_COLONNE ||
         testColonne(game->plateau, coup - 1) == -1) {
    if (coup < 1 || coup > NB_COLONNE) {
      printf("Veuillez entrer un numéro de colonne valide, entre %d et %d.\n",
             1, NB_COLONNE);
    } else
      printf("Cette colonne est pleine, veuillez en choisir une autre: ");
    clearBuffer();
    scanf("%d", &coup);
  }
  clearBuffer();
  return coup - 1;
}

/**
 * @brief Lorsque la partie est terminée, affiche le plateau et dialogue avec
 * l'utilisateur pour savoir si il veut rejouer ou non.
 *
 * @param data les données de l'interface, en mode console : inutile
 * @param game le jeu
 * @return true si l'utilisateur veut rejour
 * @return false si l'utilisateur veut arrêter
 */
static bool finDePartie(void *data, Puissance4 *game) {
  assert(game);
  if (!game->courant)
    printf("Égalité !\n");
  else
    (game->courant->type == J1) ? (printf("Joueur 1 a gagné !\n"))
                                : (printf("Joueur 2 a gagné !\n"));
  printf("Voulez-vous rejouer ? (o/n): ");
  char c = getchar();
  while (c != 'o' && c != 'O' && c != 'N' && c != 'n') {
    clearBuffer();
    printf("Entrée incorrecte. Veuillez réessayer : ");
    c = getchar();
  }
  assert(c == 'o' || c == 'O' || c == 'N' || c == 'n');
  clearBuffer();
  return (c == 'o' || c == 'O');
}

/**
 * @brief Ne fait rien, en mode console on ne peut pas rage quit.
 *
 *@param data les données de l'interface, en mode console : inutile
 */
static void destruction(void *data) {}

/**
 * @brief Crée une interface en mode console.
 *
 * @return userInterface* un pointeur sur l'inferface créée,
 * NULL en cas de problème d'allocation
 */
userInterface *makeConsole() {
  userInterface *ui = malloc(sizeof(userInterface));
  if (!ui) {
    perror("Problème d'allocation dans makeConsole.");
    return NULL;
  }
  ui->initAffichage = &printPlateau;
  ui->affichage = &printPlateau;
  ui->getProchainCoup = &prochainCoup;
  ui->endAffichage = &finDePartie;
  ui->destroy = &destruction;
  return ui;
}

/**
 * @brief Crée un joueur humain en mode console.
 *
 * @param t le type du Joueur
 * @return Joueur* un pointeur sur le Joueur créé,
 * NULL en cas de problème d'allocation
 */
Joueur *makeHumainConsole(Type t) {
  assert(t != VIDE);
  Joueur *j = malloc(sizeof(Joueur));
  if (!j) {
    perror("Problème d'allocation dans makeHumainConsole.");
    return NULL;
  }
  j->type = t;
  j->play = &playHumainConsole;
  return j;
}
/**
 * @file puissance_quatre.c
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Ensemble de fonctions de base pour le jeu puissance quatre :
 * lancer le jeu, tester si la partie est terminée, ajouter / enlever un jeton.
 * @version 0.1
 * @date 2022-12-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "puissance_quatre.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Test l'alignement de jetons à partir d'une case dans une direction
 * donnée (horizontale, verticale, diagonale dans une sens et dans l'autre)
 *
 * @param plateau le plateau de jeu
 * @param ligne le numéro de la ligne de la case
 * @param colonne le numéro de la colonne de la case
 * @param deplaL le déplacement en ligne à effectuer
 * @param deplaC le déplacemement en colonne à effectuer
 * @return true si l'alignement est supérieur ou égal au nombre de jetons pour
 * gagner (puissance 4 = 4)
 * @return false sinon
 */
bool testAlign(Plateau plateau, unsigned ligne, unsigned colonne, int deplaL,
               int deplaC) {
  assert(ligne >= 0 && ligne < NB_LIGNE);
  assert(colonne >= 0 && colonne < NB_COLONNE);
  assert(deplaL == 1 || deplaL == -1 || deplaL == 0);
  assert(deplaC == 1 || deplaC == -1 || deplaC == 0);
  Type valeur = plateau[ligne][colonne];
  assert(valeur == J1 || valeur == J2);
  unsigned nb_aligne = 1;
  int l = ligne, c = colonne;
  while (nb_aligne < NB_ALIGNE && l + deplaL >= 0 && l + deplaL < NB_LIGNE &&
         c + deplaC >= 0 && c + deplaC < NB_COLONNE &&
         plateau[l + deplaL][c + deplaC] == valeur) {
    l += deplaL;
    c += deplaC;
    nb_aligne++;
  }
  if (nb_aligne >= NB_ALIGNE)
    return true;
  l = ligne;
  c = colonne;
  while (nb_aligne < NB_ALIGNE && l - deplaL >= 0 && l - deplaL < NB_LIGNE &&
         c - deplaC >= 0 && c - deplaC < NB_COLONNE &&
         plateau[l - deplaL][c - deplaC] == valeur) {
    l -= deplaL;
    c -= deplaC;
    nb_aligne++;
  }
  return (nb_aligne >= NB_ALIGNE);
}

/**
 * @brief Test si la partie est terminée (égalité ou victoire) à partir du
 * dernier jeton joué (seule manière de gagner)
 *
 * @param game le jeu
 * @param l le numéro de ligne du dernier jeton ajouté
 * @param c le numéro de colonne du dernier jeton ajouté
 * @return true si la partie est terminée
 * @return false sinon
 */
bool testEnd(Puissance4 *game, unsigned l, unsigned c) {
  assert(game);
  assert(game->plateau[l][c] == J1 || game->plateau[l][c] == J2);
  if (testAlign(game->plateau, l, c, 0, 1)      // horizontal
      || testAlign(game->plateau, l, c, 1, 0)   // vertical
      || testAlign(game->plateau, l, c, 1, 1)   // diagonal
      || testAlign(game->plateau, l, c, 1, -1)) // diagonal
    return true;                                // joueur courant a gagné
  if (game->nb_jetons == (NB_COLONNE * NB_LIGNE)) {
    game->courant = NULL; // pour l'affichage en fin de partie
    return true;
  } // égalité
  return false;
}

/**
 * @brief Ajoute ou enlève un jeton du type précisé dans la case précisée.
 *
 * @param game le jeu
 * @param ligne le numéro de ligne de la case
 * @param colonne le numéro de colonne de la case
 * @param type le type de jeton à ajouter (peut être vide si c'est à enlever)
 */
void modifJeton(Puissance4 *game, unsigned ligne, unsigned colonne, Type type) {
  assert(game);
  assert(game->courant != NULL);
  assert(ligne >= 0 && ligne < NB_LIGNE);
  assert(colonne >= 0 && colonne < NB_COLONNE);
  if (type != VIDE) {
    assert(game->plateau[ligne][colonne] == VIDE);
    game->nb_jetons++;
  } else {
    assert(game->plateau[ligne][colonne] != VIDE);
    game->nb_jetons--;
  }
  game->plateau[ligne][colonne] = type;
}

/**
 * @brief Test si la colonne demandée est pleine.
 *
 * @param plateau le plateau de jeu
 * @param c le numéro de colonne
 * @return int -1 si la colonne est pleine, sinon le numéro de ligne de la
 * première case libre
 */
int testColonne(Plateau plateau, unsigned c) {
  assert(c >= 0 && c < NB_COLONNE);
  for (int i = NB_LIGNE - 1; i >= 0; i--) {
    if (plateau[i][c] == VIDE)
      return i;
  }
  return -1;
}

/**
 * @brief Inverser le joueur courant.
 *
 * @param game le jeu
 */
static void changerJoueur(Puissance4 *game) {
  assert(game);
  assert(game->courant);
  if (game->courant == game->j2)
    game->courant = game->j1;
  else
    game->courant = game->j2;
}

/**
 * @brief Pour recommencer une partie : le plateau à vide et le nombre de jetons
 * à 0
 *
 * @param game le jeu
 */
void initGame(Puissance4 *game) {
  assert(game);
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      game->plateau[i][j] = VIDE;
    }
  }
  game->nb_jetons = 0;
  game->courant = game->j2;
}

/**
 * @brief Récupère le prochain coup à jouer, sans tenir compte du type du joueur
 * (humain ou IA).
 *
 * @param game le jeu
 */
void prochainCoup(Puissance4 *game) {
  assert(game);
  assert(game->courant);
  unsigned coup = game->courant->play(game);
  assert(coup >= 0 && coup < NB_COLONNE);
  game->colonne = coup;
  game->ligne = testColonne(game->plateau, coup);
  assert(game->ligne != -1);
}

/**
 * @brief Lance une partie, tant qu'elle n'est pas terminée, permet de jouer.
 * Lorsque la partie se termine, permet de relancer une nouvelle partie. Gère
 * aussi les rageQuit (fermeture de la fenetre en mode interface graphique)
 *
 * @param game le jeu
 * @param ui l'interface graphique
 */
void launchGame(Puissance4 *game, userInterface *ui) {
  assert(game);
  assert(ui);
  bool rejouer;
jouer:
  initGame(game);
  ui->initAffichage(ui->data, game);
  do {
    changerJoueur(game);
    ui->getProchainCoup(game);
    if (game->rageQuit)
      return;
    modifJeton(game, game->ligne, game->colonne, game->courant->type);
    ui->affichage(ui->data, game);
  } while (!(testEnd(game, game->ligne, game->colonne)));
  rejouer = ui->endAffichage(ui->data, game);
  if (rejouer)
    goto jouer;
}

/**
 * @brief Crée un jeu du puissance 4.
 *
 * @return Puissance4* le jeu, NULL en cas de problème d'allocation
 */
Puissance4 *initPuissance4() {
  Puissance4 *game = malloc(sizeof(Puissance4));
  if (!game) {
    perror("Problème d'allocation dans initGame.");
    return NULL;
  }
  game->rageQuit = false;
  return game;
}

/**
 * @brief Supprime tout ce qui a été alloué pour le jeu du puissance 4.
 *
 * @param game le jeu
 * @param ui l'interface graphique
 */
void clean(Puissance4 *game, userInterface *ui) {
  if (game) {
    free(game->j1);
    free(game->j2);
  }
  if (ui)
    ui->destroy(ui->data);
  free(ui);
  free(game);
}

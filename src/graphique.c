#include "graphique.h"

#include <assert.h>
#include <stdio.h>

static void affichage_nomme_le_comme_tu_veux(void *data, Puissance4 *game) {
  // afficher le plateau

  // + gestion affichage fin de partie :
  /*if (game->fin) {
  if (!game->courant)
    printf("Perdu !\n");
  else
    (game->courant->c == J1) ? (printf("Joueur 1 a gagné !\n"))
                             : (printf("Joueur 2 a gagné !\n "));
}*/
}

static unsigned playHumainGraphique(void *data, Puissance4 game) {
  //
  return 0;
}
// que faire si pas ok ?
// vider buffer

static void prochainCoup(void *data, Puissance4 *game, unsigned *colonne,
                         unsigned *ligne) {
  assert(game->courant);
  unsigned coup = game->courant->play(
      NULL, *game); // NULL en fonction de l'IA... modif + tard
  while (testColonne(game->plateau, coup - 1) == -1) {
    printf("Cette colonne est pleine, veuillez en choisir une autre.");
    coup = game->courant->play(NULL, *game);
  } // à gérer dans le jouer pour la version graphique
  *colonne = coup - 1;
  *ligne = testColonne(game->plateau, coup - 1); // -1 en version console
  // modifer les valeurs de ligne et colonne (pour fc ajout jeton)
  assert(*ligne != -1);
}

userInterface *makeGraphique() {
  userInterface *ui = malloc(sizeof(userInterface));
  if (!ui) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  // ui->affichage = affichage ???;
  // ui->getProchainCoup = prochainCoup ???;
  return ui;
}

Joueur *makeHumainGraphiquee(Type c, Puissance4 *game) {
  Joueur *j = malloc(sizeof(Joueur));
  if (!j) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  j->c = c;
  j->play = &playHumainGraphique;
  return j;
}
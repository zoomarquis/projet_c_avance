#include "graphique.h"

#include "../include/SDL2/SDL.h"
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
  unsigned coup = game->courant->play(game); // NULL en fonction de l'IA... modif + tard
  while (testColonne(game->plateau, coup - 1) == -1) {
    printf("Cette colonne est pleine, veuillez en choisir une autre.");
    coup = game->courant->play(game);
  } // à gérer dans le jouer pour la version graphique
  *colonne = coup - 1;
  *ligne = testColonne(game->plateau, coup - 1); // -1 en version console
  // modifer les valeurs de ligne et colonne (pour fc ajout jeton)
  assert(*ligne != -1);
}

static int setWindowColor(SDL_Renderer *renderer, SDL_Color color)
{
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;  
}

static int initialise_plateau(SDL_Renderer *renderer){
    // Couleur de fenetre bleue
    SDL_Color bleu = {0, 102, 230, 255};

    if(setWindowColor(renderer, bleu) != 0){
      fprintf(stderr, "Erreur setWindowColor : %s", SDL_GetError());
      return -1;
    }
    return 0;
}

static int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h){
      // Initialisation SDL
    if(0 != SDL_Init(SDL_INIT_VIDEO)){
      fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
      return -1;
    }

    // Initialisation Fenetre
    *window = SDL_CreateWindow("Puissance 4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    if(NULL == window){
      fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
      return -1;
    }

    // Initialisation renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer){
      fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
      return -1;
    }

    if(0 != initialise_plateau(*renderer)){
      return -1;
    }
    return 0;
}

int draw_circle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color)
{
  int status = 0;
  if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
      return -1;
  for (int w = 0; w < radius * 2; w++)
  {
      for (int h = 0; h < radius * 2; h++)
      {
          int dx = radius - w;
          int dy = radius - h;
          if ((dx*dx + dy*dy) <= (radius * radius))
          {
              status += SDL_RenderDrawPoint(renderer, x + dx, y + dy);
              if (status < 0) {
                status = -1;
                break;
              }
          }
      }
  }
  return status;
}


static SDL_Texture** creer_tab_textures(SDL_Renderer *renderer){
  SDL_Color bleu = {0, 102, 230, 255};
  SDL_Color gris = {158, 158, 158, 255};
  SDL_Texture** tab = (SDL_Texture **) malloc(sizeof(SDL_Texture*)*42);
  for(int i = 0; i < 42; i++){
    tab[i] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 500, 500);
    if(NULL == tab[i])
    {
      fprintf(stderr, "Erreur SDL_CreateTexture : %s", SDL_GetError());
      return NULL;
    }
    SDL_SetRenderTarget(renderer, tab[i]);
    setWindowColor(renderer, bleu);
    draw_circle(renderer, 20, 20, 30, gris);
    SDL_SetRenderTarget(renderer, NULL);
  }

  return tab;
}

userInterface *makeGraphique() {
  userInterface *ui = malloc(sizeof(userInterface));
  if (!ui) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  if(0 != init(&window, &renderer, 640, 480)){
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  SDL_Texture **tab = creer_tab_textures(renderer);
/*
  for(int i = 0; i < 42; i++){
      SDL_DestroyTexture(tab[i]);
      free(tab[i]);
    }
*/

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
  j->type = c;
  j->play = &playHumainGraphique;
  return j;
}
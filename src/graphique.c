/**
 * @file graphique.c
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Ensemble de fonctions pour le mode d'interface graphique du jeu : créer,
 * afficher, récupérer l'action...
 * @version 1.0
 * @date 2022-12-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "graphique.h"

#include "../include/SDL2/SDL.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @def WIDTH
 * @brief Largeur de la fenêtre
*/
/**
 * @def HEIGHT
 * @brief Hauteur de la fenêtre
*/
#define WIDTH 1200
#define HEIGHT 900


/**
 * @struct _SDLData
 * @brief Contient les éléments principaux de l'interface graphique
 * @typedef SDLData _SDLData
 */
typedef struct _SDLData {
  SDL_Renderer *renderer;   //<! Pointeur sur le renderer
  SDL_Window *window;   //<! Pointeur sur la fenêtre
  SDL_Texture *tab_texture[NB_LIGNE][NB_COLONNE];   //<! Pointeur sur le tableau des textures
} SDLData;


/**
 * @brief Permet de definir la couleur de fond du renderer donné.
 *
 * @param renderer Pointeur sur le renderer en question
 * @param color La couleur de fond
 * 
 * @return int retourne 0 si tout s'est bien passé, -1 sinon
 */
static int setRendererColor(SDL_Renderer *renderer, SDL_Color color) {
  if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
    return -1;
  if (SDL_RenderClear(renderer) < 0)
    return -1;
  return 0;
}

/**
 * @brief Permet de detruire la SDL proprement.
 *
 * @param window Pointeur sur la fenêtre
 * @param renderer Pointeur sur le renderer
 * @param tab_texture Pointeur sur le tableau des textures
 */
static void destroySDL(SDL_Window *window, SDL_Renderer *renderer,
                       SDL_Texture *tab_texture[NB_LIGNE][NB_COLONNE]) {
  if (NULL != renderer)
    SDL_DestroyRenderer(renderer);
  if (NULL != window)
    SDL_DestroyWindow(window);
  if (NULL != tab_texture) {
    for (int i = 0; i < NB_LIGNE; i++) {
      for (int j = 0; j < NB_COLONNE; j++) {
        SDL_DestroyTexture(tab_texture[i][j]);
      }
    }
  }
  SDL_Quit();
}

/**
 * @brief Permet de detruire les données du jeu.
 *
 * @param data Pointeur les données de l'interface graphique
 */
static void destroyData(void *data){
  SDLData *d = (SDLData *)data;
  destroySDL(d->window, d->renderer, d->tab_texture);
  free(d);
}

/**
 * @brief Cette fonction est un EventFilter c'est a dire qu'elle est appelée
 * lorsque l'event Quit est reconnu, elle permet de quitter la SDL lorsque l'utilisateur ragequit
 *
 * @param userdata Pointeur le booleen ragequit
 * @return int valeur de retour obligatoire mais inutile pour nous (utile pour la SDL)
 */
static int EventQuit(void *userdata, SDL_Event *event) {
  bool *rQ = (bool *)userdata;
  if (event->type == SDL_QUIT) {
    *rQ = true;
  }
  return 0;
}

/**
 * @brief Permet au joueur humain de jouer un pion.
 *
 * @param userdata Pointeur le booleen ragequit
 * @param game le jeu
 * 
 * @return unsigned la colonne où le joueur place un pion
 */
static unsigned playHumainGraphique(Puissance4 *game) {
  int coup;
  int grid_cell_width = (WIDTH / NB_COLONNE) - 30;
  int width_plateau = WIDTH - (30 * NB_COLONNE);
  assert(game->courant);
  SDL_Event event;
  SDL_bool joue = SDL_FALSE;
  while (!joue) {
    SDL_WaitEvent(&event);
    if (event.type == SDL_MOUSEBUTTONUP && event.button.x <= width_plateau) {
      coup = (event.button.x) / (grid_cell_width);
      if (testColonne(game->plateau, coup) != -1) {
        joue = SDL_TRUE;
      }
    }
  }
  return coup;
}

/**
 * @brief Récupère le prochain coup à jouer, sans tenir compte du type du joueur
 * (humain ou IA).
 *
 * @param game le jeu
 */
static void prochainCoup(Puissance4 *game) {
  assert(game->courant);
  unsigned coup = game->courant->play(game);
  game->colonne = coup;
  game->ligne = testColonne(game->plateau, coup);
  assert(game->ligne != -1);
}


/**
 * @brief Perme.
 *
 * @param userdata Pointeur le booleen ragequit
 * @param game le jeu
 * 
 * @return unsigned la colonne où le joueur place un pion
 */
static int initialise_plateau(SDL_Renderer *renderer) {
  // Couleur de fenetre blanche
  SDL_Color blanc = {255, 255, 255, 255};

  if (setRendererColor(renderer, blanc) != 0) {
    fprintf(stderr, "Erreur setRendererColor : %s", SDL_GetError());
    return -1;
  }
  return 0;
}

static int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h) {
  // Initialisation SDL
  if (0 != SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
    return -1;
  }

  // Initialisation Fenetre
  *window = SDL_CreateWindow("Puissance 4", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
  if (NULL == window) {
    fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
    return -1;
  }

  // Initialisation renderer
  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
  if (NULL == renderer) {
    fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
    return -1;
  }

  if (0 != initialise_plateau(*renderer)) {
    return -1;
  }
  return 0;
}

static int draw_circle(SDL_Renderer *renderer, int x, int y, int radius,
                SDL_Color color) {
  int status = 0;
  if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
    return -1;
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w;
      int dy = radius - h;
      if ((dx * dx + dy * dy) <= (radius * radius)) {
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

static void updateGraphique(void *data, Puissance4 *game) {
  SDLData *d = (SDLData *)data;
  int grid_cell_width = (WIDTH / NB_COLONNE) - 30;
  int grid_cell_height = HEIGHT / NB_LIGNE;
  SDL_Color blanc = {255, 255, 255, 255};
  SDL_Color jaune = {227, 195, 16, 255};
  SDL_Color rouge = {222, 61, 40, 255};
  SDL_Rect rect = {(game->colonne * grid_cell_width) + 5,
                   (game->ligne * grid_cell_height) + 5, grid_cell_width - 10,
                   grid_cell_height - 10};

  SDL_SetRenderTarget(d->renderer, d->tab_texture[game->ligne][game->colonne]);
  setRendererColor(d->renderer, blanc);
  switch (game->plateau[game->ligne][game->colonne]) {
  case J1:
    draw_circle(d->renderer, 250, 250, 240, rouge);
    SDL_SetRenderTarget(d->renderer, NULL);
    SDL_RenderCopy(d->renderer, d->tab_texture[game->ligne][game->colonne],
                   NULL, &rect);
    break;
  case J2:
    draw_circle(d->renderer, 250, 250, 240, jaune);
    SDL_SetRenderTarget(d->renderer, NULL);
    SDL_RenderCopy(d->renderer, d->tab_texture[game->ligne][game->colonne],
                   NULL, &rect);
    break;
  case VIDE:
    break;
  }

  SDL_RenderPresent(d->renderer);
  SDL_Delay(300);
}

static void initPlateauGraphique(void *data, Puissance4 *game) {
  // afficher le plateau
  SDLData *d = (SDLData *)data;
  int grid_cell_width = (WIDTH / NB_COLONNE) - 30;
  int grid_cell_height = HEIGHT / NB_LIGNE;
  SDL_Color blanc = {255, 255, 255, 255};
  SDL_Color noir = {0, 0, 0, 255};
  SDL_RenderClear(d->renderer);
  setRendererColor(d->renderer, blanc);
  SDL_SetRenderDrawColor(d->renderer, noir.r, noir.g, noir.b, noir.a);
  SDL_Rect rect;
  for (int l = 0; l < NB_LIGNE; l++) {
    for (int c = 0; c < NB_COLONNE; c++) {
      rect.x = c * grid_cell_width;
      rect.y = l * grid_cell_height;
      rect.w = grid_cell_width;
      rect.h = grid_cell_height;
      SDL_RenderDrawRect(d->renderer, &rect);
    }
  }

  SDL_RenderPresent(d->renderer);
}

static int creer_tab_textures(SDLData *d, SDL_Renderer *renderer) {
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      d->tab_texture[i][j] =
          SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_TARGET, 500, 500);
      if (NULL == d->tab_texture[i][j]) {
        fprintf(stderr, "Erreur SDL_CreateTexture : %s", SDL_GetError());
        for(int k = 0; k <= i; k++){
          for(int l = 0; l < j; l++){
            SDL_DestroyTexture(d->tab_texture[k][l]);
          }
        }
        return -1;
      }
    }
  }
  return 0;
}

static bool endAffichage();

userInterface *makeGraphique() {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  userInterface *ui = malloc(sizeof(userInterface));

  if (!ui) {
    perror("Problème d'allocation dans makeGraphique.");
    return NULL;
  }

  if (0 != init(&window, &renderer, WIDTH, HEIGHT)) {
    free(ui);
    destroySDL(window, renderer, NULL);
    return NULL;
  }

  SDLData *d = malloc(sizeof(SDLData));
  if (!d) {
    perror("Problème d'allocation dans makeGraphique.");
    free(ui);
    destroySDL(window, renderer, NULL);
    return NULL;
  }

  d->renderer = renderer;
  d->window = window;

  if(0 != creer_tab_textures(d, renderer)){
    free(ui);
    free(d);
    destroySDL(window, renderer, NULL);
    return NULL;
  }

  SDL_AddEventWatch(EventQuit, d);

  ui->data = d;
  ui->rageQuit = false;
  ui->initAffichage = initPlateauGraphique;
  ui->affichage = updateGraphique;
  ui->getProchainCoup = prochainCoup;
  ui->destroy = destroyData;
  ui->endAffichage = NULL;

  return ui;
}

Joueur *makeHumainGraphique(Type c) {
  Joueur *j = malloc(sizeof(Joueur));
  if (!j) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }
  j->type = c;
  j->play = &playHumainGraphique;
  return j;
}
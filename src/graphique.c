#include "graphique.h"

#include "../include/SDL2/SDL.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 1200
#define HEIGHT 900

typedef struct _SDLData {
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Texture *tab_texture[NB_LIGNE][NB_COLONNE];
} SDLData;

static int setWindowColor(SDL_Renderer *renderer, SDL_Color color) {
  if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
    return -1;
  if (SDL_RenderClear(renderer) < 0)
    return -1;
  return 0;
}

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

int EventQuit(void *userdata, SDL_Event *event) {
  // SDLData *d = (SDLData *)userdata;
  //  bool rageQuit à passer à vrai
  if (event->type == SDL_QUIT) {
    // rageQuit à vrai;
    // destroySDL(d->window, d->renderer, d->tab_texture);
  }
  return 0;
}

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

static void prochainCoup(void *data, Puissance4 *game) {
  assert(game->courant);
  unsigned coup = game->courant->play(game);
  game->colonne = coup;
  game->ligne = testColonne(game->plateau, coup);
  assert(game->ligne != -1);
}

static int initialise_plateau(SDL_Renderer *renderer) {
  // Couleur de fenetre bleue
  SDL_Color blanc = {255, 255, 255, 255};

  if (setWindowColor(renderer, blanc) != 0) {
    fprintf(stderr, "Erreur setWindowColor : %s", SDL_GetError());
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

int draw_circle(SDL_Renderer *renderer, int x, int y, int radius,
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
  setWindowColor(d->renderer, blanc);
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
  setWindowColor(d->renderer, blanc);
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

static void creer_tab_textures(SDLData *d, SDL_Renderer *renderer) {
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      d->tab_texture[i][j] =
          SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_TARGET, 500, 500);
      if (NULL == d->tab_texture[i][j]) {
        fprintf(stderr, "Erreur SDL_CreateTexture : %s", SDL_GetError());
        return;
      }
    }
  }
}

bool endAffichage();

userInterface *makeGraphique() {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  userInterface *ui = malloc(sizeof(userInterface));

  if (!ui) {
    perror("Problème d'allocation.");
    exit(EXIT_FAILURE);
  }

  if (0 != init(&window, &renderer, WIDTH, HEIGHT)) {
    destroySDL(window, renderer, NULL);
  }

  SDLData *d = malloc(sizeof(SDLData));
  d->renderer = renderer;
  d->window = window;
  creer_tab_textures(d, renderer);

  SDL_AddEventWatch(EventQuit, d);

  ui->data = d;
  ui->rageQuit = false;
  ui->initAffichage = initPlateauGraphique;
  ui->affichage = updateGraphique;
  ui->getProchainCoup = prochainCoup;
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
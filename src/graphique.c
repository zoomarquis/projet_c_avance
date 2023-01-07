/**
 * @file graphique.c
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Ensemble de fonctions pour le mode d'interface graphique du jeu :
 * créer, afficher, récupérer l'action...
 * @version 0.1
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
#include <stdlib.h>

/**
 * @def WIDTH
 * @brief Largeur de la fenêtre
 */
/**
 * @def HEIGHT
 * @brief Hauteur de la fenêtre
 */
/**
 * @def PAS
 * @brief La quantité de pixels deduite de la largeur de chaque case du tableau
 * pour pouvoir avec un espace a droite du plateau dans la fenêtre
 */
#define WIDTH 1500
#define HEIGHT 900
#define PAS 50

/**
 * @struct _SDLData
 * @brief Contient les éléments principaux de l'interface graphique
 * @typedef SDLData
 * @brief Renommer _SDLData
 */
typedef struct _SDLData {
  SDL_Renderer *renderer;                         //!< Pointeur sur le renderer
  SDL_Window *window;                             //!< Pointeur sur la fenêtre
  SDL_Texture *tab_texture[NB_LIGNE][NB_COLONNE]; //!< Pointeur sur le tableau
                                                  //!< des textures
  SDL_Texture *tour1; //!< Pointeur sur l'image pour le tour du joueur 1
  SDL_Texture *tour2; //!< Pointeur sur l'image pour le tour du joueur 2
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
  if (NULL != tab_texture) {
    for (int i = 0; i < NB_LIGNE; i++) {
      for (int j = 0; j < NB_COLONNE; j++) {
        if (tab_texture[i][j] != NULL)
          SDL_DestroyTexture(tab_texture[i][j]);
      }
    }
  }
  if (NULL != renderer)
    SDL_DestroyRenderer(renderer);
  if (NULL != window)
    SDL_DestroyWindow(window);

  SDL_Quit();
}

/**
 * @brief Permet de detruire les données du jeu.
 *
 * @param data Pointeur les données de l'interface graphique
 */
static void destroyData(void *data) {
  SDLData *d = (SDLData *)data;
  if(d){
    if(d->tour1 != NULL){
      SDL_DestroyTexture(d->tour1);
    }

    if(d->tour2 != NULL){
      SDL_DestroyTexture(d->tour2);
    }

    destroySDL(d->window, d->renderer, (d->tab_texture));
    free(d);

    }
}

/**
 * @brief Permet de mettre la fenetre en blanc
 *
 * @param renderer Pointeur sur le renderer
 * @return int 0 si le changement a ete effectue -1 sinon
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


/**
 * @brief Permet de creer le tableau de textures
 *
 * @param d Le pointeur sur la data de la SDL (window, renderer, tab_texture)
 * @param renderer Le pointeur sur le renderer
 *
 * @return int 0 si tout s'est bien passé, -1 sinon
 */
static int creer_tab_textures(SDLData *d) {
  int tmp = 0;
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      d->tab_texture[i][j] =
          SDL_CreateTexture(d->renderer, SDL_PIXELFORMAT_RGBA8888,
                            SDL_TEXTUREACCESS_TARGET, 500, 500);
      if (NULL == d->tab_texture[i][j]) {
        fprintf(stderr, "Erreur SDL_CreateTexture : %s", SDL_GetError());
        tmp = j;
        for (int k = 0; k <= i; k++) {
          if(k != i){
            j = NB_COLONNE;
          }else{
            j = tmp;
          }
          for (int l = 0; l < j; l++) {
            if(d->tab_texture[k][l] != NULL){
              SDL_DestroyTexture(d->tab_texture[k][l]);
              d->tab_texture[k][l] = NULL;
            }
          }
        }
        return -1;
      }
    }
  }
  return 0;
}

/**
 * @brief Permet d'initialiser la SDL, notre fenetre et notre renderer
 *
 * @param window Le pointeur sur la fenêtre qui va être créee
 * @param renderer Le pointeur sur le renderer qui va être crée
 * @param w La largeur de la fenêtre
 * @param h La hauteur de la fenêtre
 * @return int 0 si tout s'est bien passé, -1 sinon
 */
static int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h) {
  // Initialisation SDL
  if (0 != SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
    return -2;
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
/**
 * @brief Permet de faire le quadrillage 6x7 de la fenêtre
 *
 * @param data Le pointeur sur la data de la SDL (window, renderer, tab_texture)
 * @param game Le pointeur sur le jeu
 */
static void initPlateauGraphique(void *data, Puissance4 *game) {
  // afficher le plateau
  SDLData *d = (SDLData *)data;
  int grid_cell_width = (WIDTH / NB_COLONNE) - PAS;
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

  SDL_Rect rectTour = {WIDTH - 270, HEIGHT - 850, 200, 200};
  SDL_RenderCopy(d->renderer, d->tour2, NULL, &rectTour);
  SDL_RenderPresent(d->renderer);
}

/**
 * @brief Permet au joueur humain en mode graphique de jouer un pion.
 *
 * @param game le jeu
 * @return unsigned la colonne où le joueur place un pion
 */
static unsigned playHumainGraphique(Puissance4 *game) {
  int coup = 0;
  int grid_cell_width = (WIDTH / NB_COLONNE) - PAS;
  int width_plateau = (grid_cell_width * NB_COLONNE);
  assert(game->courant);
  SDL_Event event;
  SDL_bool joue = SDL_FALSE;
  while (!joue) {
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT) {
      game->rageQuit = true;
      return coup;
    }
    if (event.type == SDL_MOUSEBUTTONUP && event.button.x < width_plateau) {
      coup = (event.button.x) / (grid_cell_width);
      if (testColonne(game->plateau, coup) != -1) {
        joue = SDL_TRUE;
      }
    }
  }
  return coup;
}

/**
 * @brief Permet de dessiner un cercle
 *
 * @param renderer Le pointeur sur le renderer qui va être crée
 * @param x Centre du cercle en abscisse
 * @param y Centre du cercle en ordonnée
 * @param radius Le rayon du cercle
 * @param color La couleur du cercle
 * @return int 0 si tout s'est bien passé, -1 sinon
 */
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

/**
 * @brief Permet de mettre a jour la fenêtre lorsqu'un coup est joué
 *
 * @param data Le pointeur sur la data de la SDL (window, renderer, tab_texture)
 * @param game Le pointeur sur le jeu
 */
static void updateGraphique(void *data, Puissance4 *game) {
  SDLData *d = (SDLData *)data;
  int grid_cell_width = (WIDTH / NB_COLONNE) - PAS;
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

  SDL_Rect rectTour = {WIDTH - 270, HEIGHT - 850, 200, 200};
  switch (game->courant->type) {
  case J1:
    SDL_RenderCopy(d->renderer, d->tour1, NULL, &rectTour);
    break;

  case J2:
    SDL_RenderCopy(d->renderer, d->tour2, NULL, &rectTour);
    break;

  case VIDE:
    break;
  }

  SDL_RenderPresent(d->renderer);
  SDL_Delay(100);
}

/**
 * @brief Permet de faire l'affichage de fin de partie
 *
 * @param data Le pointeur sur la data de la SDL (window, renderer, tab_texture)
 * @param game Le pointeur sur le jeu
 *
 * @return bool false si le joueur ne souhaite pas rejouer, true si il veut
 * rejouer
 */
static bool endAffichage(void *data, Puissance4 *game) {
  SDLData *d = (SDLData *)data;
  SDL_Surface *egalite = NULL;
  SDL_Surface *gagne = NULL;
  SDL_Surface *rejouer = NULL;
  SDL_Texture *egaliteT = NULL;
  SDL_Texture *gagneT = NULL;
  SDL_Texture *rejouerT = NULL;
  SDL_Rect rect = {WIDTH - 300, HEIGHT - 500, 250, 100};

  SDL_Surface *blanc = SDL_LoadBMP("img/blanc.bmp");
  if (!blanc) {
    fprintf(stderr, "Erreur de chargement de la surface : %s", SDL_GetError());
    return false;
  }

  SDL_Texture *blancT = SDL_CreateTextureFromSurface(d->renderer, blanc);
  SDL_FreeSurface(blanc);
  if (!blancT) {
    fprintf(stderr, "Erreur de chargement de la texture : %s", SDL_GetError());
    return false;
  }

  SDL_Rect rectTour = {WIDTH - 270, HEIGHT - 850, 200, 200};
  SDL_RenderCopy(d->renderer, blancT, NULL, &rectTour);

  if (!game->courant) {
    egalite = SDL_LoadBMP("img/egalite.bmp");
    if (!egalite) {
      fprintf(stderr, "Erreur de chargement de la surface : %s",
              SDL_GetError());
      SDL_DestroyTexture(blancT);
      return false;
    }
    egaliteT = SDL_CreateTextureFromSurface(d->renderer, egalite);
    SDL_FreeSurface(egalite);
    if (!egaliteT) {
      fprintf(stderr, "Erreur de chargement de la texture : %s",
              SDL_GetError());
      SDL_DestroyTexture(blancT);
      return false;
    }

    rejouer = SDL_LoadBMP("img/rejouer.bmp");
    if (!rejouer) {
      fprintf(stderr, "Erreur de chargement de la surface : %s",
              SDL_GetError());
      SDL_DestroyTexture(egaliteT);
      SDL_DestroyTexture(blancT);
      return false;
    }
    rejouerT = SDL_CreateTextureFromSurface(d->renderer, rejouer);
    SDL_FreeSurface(rejouer);
    if (!rejouerT) {
      fprintf(stderr, "Erreur de chargement de la texture : %s",
              SDL_GetError());
      SDL_DestroyTexture(egaliteT);
      SDL_DestroyTexture(blancT);
      return false;
    }

    SDL_Rect rect2 = {WIDTH - 300, HEIGHT - 200, 250, 100};
    SDL_RenderCopy(d->renderer, rejouerT, NULL, &rect2);

    SDL_RenderCopy(d->renderer, egaliteT, NULL, &rect);
    SDL_RenderPresent(d->renderer);
  } else {
    if (game->courant->type == J1) {
      gagne = SDL_LoadBMP("img/gagnerouge.bmp");
    } else {
      gagne = SDL_LoadBMP("img/gagnejaune.bmp");
    }

    if (!gagne) {
      printf("Erreur de chargement de l'image : %s", SDL_GetError());
      SDL_DestroyTexture(blancT);
      return false;
    }

    gagneT = SDL_CreateTextureFromSurface(d->renderer, gagne);
    SDL_FreeSurface(gagne);
    if (!gagneT) {
      printf("Erreur de chargement de la surface : %s", SDL_GetError());
      SDL_DestroyTexture(blancT);
      return false;
    }

    SDL_RenderCopy(d->renderer, gagneT, NULL, &rect);

    rejouer = SDL_LoadBMP("img/rejouer.bmp");
    if (!rejouer) {
      fprintf(stderr, "Erreur de chargement de la surface : %s",
              SDL_GetError());
      SDL_DestroyTexture(gagneT);
      SDL_DestroyTexture(blancT);
      return false;
    }
    rejouerT = SDL_CreateTextureFromSurface(d->renderer, rejouer);
    SDL_FreeSurface(rejouer);
    if (!rejouerT) {
      fprintf(stderr, "Erreur de chargement de la texture : %s",
              SDL_GetError());
      SDL_DestroyTexture(gagneT);
      SDL_DestroyTexture(blancT);
      return false;
    }

    SDL_Rect rect2 = {WIDTH - 300, HEIGHT - 200, 250, 100};
    SDL_RenderCopy(d->renderer, rejouerT, NULL, &rect2);
    SDL_RenderPresent(d->renderer);
  }

  SDL_Event event;
  SDL_bool action = SDL_FALSE;

  while (!action) {
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT) {
      action = SDL_TRUE;
      if (egalite) {
        SDL_DestroyTexture(egaliteT);
      }
      if (gagne) {
        SDL_DestroyTexture(gagneT);
      }
      if (rejouer) {
        SDL_DestroyTexture(rejouerT);
      }

      if (blanc) {
        SDL_DestroyTexture(blancT);
      }
      // Free les surfaces et textures créées au dessus
      return false;
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
      // Hitbox du bouton rejouer
      if ((event.button.x >= WIDTH - 300 && event.button.x <= WIDTH - 50) &&
          (event.button.y >= HEIGHT - 200 && event.button.y <= HEIGHT - 100)) {
        action = SDL_TRUE;
        if (egalite) {
          SDL_DestroyTexture(egaliteT);
        }
        if (gagne) {
          SDL_DestroyTexture(gagneT);
        }
        if (rejouer) {
          SDL_DestroyTexture(rejouerT);
        }

        if (blanc) {
          SDL_DestroyTexture(blancT);
        }
        return true;
      }
    }
  }
  return false;
}

/**
 * @brief Crée une interface en mode graphique.
 *
 * @return userInterface* un pointeur sur l'inferface créée,
 * NULL en cas de problème d'allocation
 */
userInterface *makeGraphique() {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  userInterface *ui = malloc(sizeof(userInterface));

  if (!ui) {
    perror("Problème d'allocation dans makeGraphique.");
    return NULL;
  }

  int status = init(&window, &renderer, WIDTH, HEIGHT);

  if (status != 0) {
    free(ui);
    if(status != -2){
      destroySDL(window, renderer, NULL);
    }
    return NULL;
  }

  SDLData *d = malloc(sizeof(SDLData));
  if (!d) {
    perror("Problème d'allocation dans makeGraphique.");
    destroySDL(window, renderer, NULL);
    free(ui);
    return NULL;
  }

  d->renderer = renderer;
  d->window = window;

  SDL_Surface *tour1 = SDL_LoadBMP("img/jaune.bmp");
  if (!tour1) {
    fprintf(stderr, "Erreur de chargement de la surface : %s", SDL_GetError());
    destroySDL(window, renderer, NULL);
    free(ui);
    free(d);
    return NULL;
  }

  SDL_Texture *tour1T = SDL_CreateTextureFromSurface(renderer, tour1);
  SDL_FreeSurface(tour1);
  if (!tour1T) {
    fprintf(stderr, "Erreur de chargement de la texture : %s", SDL_GetError());
    destroySDL(window, renderer, NULL);
    free(ui);
    free(d);
    return NULL;
  }

  SDL_Surface *tour2 = SDL_LoadBMP("img/rouge.bmp");
  if (!tour2) {
    fprintf(stderr, "Erreur de chargement de la surface : %s", SDL_GetError());
    SDL_DestroyTexture(tour1T);
    destroySDL(window, renderer, NULL);
    free(ui);
    free(d);
    return NULL;
  }

  SDL_Texture *tour2T = SDL_CreateTextureFromSurface(renderer, tour2);
  SDL_FreeSurface(tour2);
  if (!tour2T) {
    fprintf(stderr, "Erreur de chargement de la texture : %s", SDL_GetError());
    SDL_DestroyTexture(tour1T);
    destroySDL(window, renderer, NULL);
    free(ui);
    free(d);
    return NULL;
  }

  d->tour1 = tour1T;
  d->tour2 = tour2T;

  if (0 != creer_tab_textures(d)) {
    free(ui);
    free(d);
    SDL_DestroyTexture(tour1T);
    SDL_DestroyTexture(tour2T);
    destroySDL(window, renderer, NULL);
    return NULL;
  }

  ui->data = d;
  ui->initAffichage = &initPlateauGraphique;
  ui->affichage = &updateGraphique;
  ui->getProchainCoup = &prochainCoup;
  ui->destroy = &destroyData;
  ui->endAffichage = &endAffichage;

  return ui;
}

/**
 * @brief Crée un joueur humain en mode graphique.
 *
 * @param t le type du Joueur
 * @return Joueur* un pointeur sur le Joueur créé,
 * NULL en cas de problème d'allocation
 */
Joueur *makeHumainGraphique(Type t) {
  assert(t != VIDE);
  Joueur *j = malloc(sizeof(Joueur));
  if (!j) {
    perror("Problème d'allocation dans makeHumainConsole.");
    return NULL;
  }
  j->type = t;
  j->play = &playHumainGraphique;
  return j;
}
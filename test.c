#include "include/SDL2/SDL.h"

#include <stdlib.h>


int setWindowColor(SDL_Renderer *renderer, SDL_Color color)
{
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
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

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;
    SDL_Color bleu = {0, 102, 230, 255};
    SDL_Color noir = {0, 0, 0, 255};

    if(0 != SDL_Init(SDL_INIT_VIDEO)){
      fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
      goto Quit;
    }

    window = SDL_CreateWindow("Puissance 4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if(NULL == window){
      fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
      goto Quit;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer){
      fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
      goto Quit;
    }

    if(setWindowColor(renderer, bleu) != 0){
      fprintf(stderr, "Erreur setWindowColor : %s", SDL_GetError());
      goto Quit;
    }

    int x = 30;
    for(int i = 1; i < 8; i++){
      draw_circle(renderer, 100, x, 30, noir);
      x += 60;
    }
    



    statut = EXIT_SUCCESS;
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);

Quit:
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}
#include "include/SDL2/SDL.h"

#include <stdlib.h>

int main() {
  SDL_Window *window = NULL;
  SDL_Init(SDL_INIT_AUDIO);
  window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
  SDL_Delay(3000);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
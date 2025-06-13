#include <SDL2/SDL.h>


int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow(
    "test SDL",
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED,
    640, 
    480, 
    SDL_WINDOW_SHOWN
  );
  auto screen = SDL_GetWindowSurface(window);
  SDL_Event event;
  bool run = true;
  while (run) {
    while(SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        run = false;
      }
    }
    
    SDL_FillRect(screen, NULL, 0);
    /// draw
    SDL_UpdateWindowSurface(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
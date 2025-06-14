//#define DEBUG_STACKCONVEX
#define DEBUG_TRAPEZOID
#include "../src/eng3d/SDL.h"
#include "../src/eng3d/StackConvex.h"
#include "../src/eng3d/ContextOneColor.h"
#include "../src/geom/Matrix.h"


#include <SDL2/SDL.h>
#include <thread>
#include <chrono>
#include <cmath>

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow(
    "test StackConvex",
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED,
    640, 
    480, 
    SDL_WINDOW_SHOWN
  );
  auto screen = SDL_GetWindowSurface(window);
  
  auto surface = eng3d::buildSurface(screen);
  
  eng3d::StackConvex<float, 5> convex{
    geom::Vector(320.f, 50),
    geom::Vector(420.f, 100),
    geom::Vector(370.f, 280),
    geom::Vector(270.f, 250),
    geom::Vector(220.f, 110)
  };
  
  auto wrap = geom::WrapMatrix(convex);
  
  

  SDL_Event event;
  bool run = true;
  while (run) {
    while(SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        run = false;
      }
    }
    
    SDL_FillRect(screen, NULL, 0);

    surface.draw(
      convex,
      eng3d::ContextOneColor(
        SDL_Color(255, 0, 0)
      ) 
    );
    geom::Vector center(520.f, 240);
    float angle = 0.0003;
    float sin1 = std::sin(angle);
    float cos1 = std::cos(angle);
    geom::Matrix<float, 2, 2> mat;
    mat(0, 0) = cos1;
    mat(0, 1) = -sin1;
    mat(1, 0) = sin1; 
    mat(1, 1) = cos1;
    
    
    convex -= center;
    wrap *= mat;
    convex += center;

    convex.normalize();
    
  
    /*std::this_thread::sleep_for(
      std::chrono::milliseconds(6)
    );*/
    SDL_UpdateWindowSurface(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
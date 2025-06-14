#define DEBUG_TRAPEZOID_LONGLINES
#include "../src/eng3d/SDL.h"
#include "../src/eng3d/Trapezoid.h"
#include "../src/eng3d/ContextOneColor.h"

#include <SDL2/SDL.h>
#include <thread>
#include <chrono>
#include <iostream>


int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow(
    "test Trapezoid",
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED,
    640, 
    480, 
    SDL_WINDOW_SHOWN
  );
  auto screen = SDL_GetWindowSurface(window);
  
  auto surface = eng3d::buildSurface(screen);
  
  geom::Line left(geom::Vector(30, 20), geom::Vector(10, 100));
  geom::Line right(geom::Vector(200, 20), geom::Vector(400, 100));
  left.normalize();
  right.normalize();
  left += geom::Vector(-500, 0);
  right += geom::Vector(-500, 0);
  eng3d::Trapezoid trapezoid(20, 100, left, right);

  geom::Line left2(geom::Vector(10.f, 120), geom::Vector(30.f, 200));
  geom::Line right2(geom::Vector(400.f, 120), geom::Vector(200.f, 200));
  left2.normalize();
  right2.normalize();
  left2 += geom::Vector(-500.f, 0);
  right2 += geom::Vector(-500.f, 0);
  eng3d::Trapezoid trapezoid2(120, 200, left2, right2);
  
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
      trapezoid, 
      eng3d::ContextOneColor(
        SDL_Color(255, 255, 255) 
      )
    );

    surface.draw(
      trapezoid2, 
      eng3d::ContextOneColor(
        SDL_Color(255, 0, 0)
      )
    );
    left += geom::Vector(1, 0);
    right += geom::Vector(1, 0);
    left2 += geom::Vector(2.f, 0);
    right2 += geom::Vector(2.f, 0);
    std::this_thread::sleep_for(
      std::chrono::milliseconds(6)
    );
    SDL_UpdateWindowSurface(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
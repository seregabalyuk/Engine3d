#include "../src/eng3d/Camera.h"
#include "../src/eng3d/SDL.h"
#include "../src/eng3d/ContextOneColor.h"

#include <iostream>

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow(
    "test simple triangle",
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED,
    640, 
    480, 
    SDL_WINDOW_SHOWN
  );
  auto screen = SDL_GetWindowSurface(window);
  SDL_Event event;
  bool run = true;

  auto surface = eng3d::buildSurface(screen);
  

  geom::Vector<int, 2> mousePos(0, 0);
  SDL_GetMouseState(&mousePos.x, &mousePos.y);

  eng3d::Camera camera(1.f, 1.f, 1.f);

  geom::Triangle triangle( 
    geom::Vector(-0.5f, -0.5, 2),
    geom::Vector(-0.5f, 0.5, 2),
    geom::Vector(0.5f, -0.5, 2)
  );

  int state = 1;
  
  
  while (run) {
    while(SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        run = false;
      }
    }
    geom::Vector<int, 2> next;
    SDL_GetMouseState(&next.x, &next.y);
    mousePos = next;
    SDL_FillRect(screen, NULL, 0);
    bool out = 0;
    for (int i = 0; i < 3; ++ i) {
      if ((1<<i) & state) {
        triangle.points[i].z -= 0.0003f;
      }
      if (triangle.points[i].z < -2) {
        out = true;
      }
    }
    if (out) {
      ++ state;
      for (int i = 0; i < 3; ++ i) {
        triangle.points[i].z = 2;
      }
      if (state == 8) {
        state = 1;
      }
    }

    auto next_triangle = camera.apply(triangle);
    geom::Vector<float, 2> points[4];
    int s = camera.project(next_triangle, points);
    if (s == 3) {
      auto& convex = reinterpret_cast<
        eng3d::StackConvex<float, 3>&
      >(points);
      convex += geom::Vector(1.f, 1.f);
      float w = surface.width / 2;
      float h = surface.height / 2;
      for(auto& point: convex) {
        point.x *= w;
        point.y *= h;
      }
      convex.normalizeLight();      
      surface.draw(
        convex,
        eng3d::ContextOneColor(
          SDL_Color(0, 255, 255)
        )
      );
    } else if (s == 4) {
      auto& convex = reinterpret_cast<
        eng3d::StackConvex<float, 4>&
      >(points);
      convex += geom::Vector(1.f, 1.f);
      float w = surface.width / 2;
      float h = surface.height / 2;
      for(auto& point: convex) {
        point.x *= w;
        point.y *= h;
      }
      convex.normalizeLight();      
      surface.draw(
        convex,
        eng3d::ContextOneColor(
          SDL_Color(0, 255, 255)
        )
      );
    }
    
   
    SDL_UpdateWindowSurface(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
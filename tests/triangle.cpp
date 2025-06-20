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

  geom::Triangle triangle2( 
    geom::Vector(-0.5f, -0.5, 2),
    geom::Vector(-0.5f, 0.5, 2),
    geom::Vector(0.5f, -0.5, 2)
  );

  int state = 1;
  auto mat = camera.matrix;
  float angleX = 0;
  float angleY = 0;
  
  int alien_x = 0;
  int alien_y = 0;
  
  
  while (run) {
    while(SDL_PollEvent(&event) != 0) {
      switch( event.type ) {
        case SDL_QUIT:
          run = false;
          break;
      }
    }
    

    geom::Vector<int, 2> next;
    SDL_GetMouseState(&next.x, &next.y);
    angleX += next.x - mousePos.x;
    angleY += next.y - mousePos.y;
    float coef = 0.02;
    camera.matrix = mat;
    camera.matrix.rotate(0, 1, angleX * coef);
    camera.matrix.rotate(1, 2, angleY * coef);
    mousePos = next;

    auto keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_W]) {
      geom::Matrix<float, 2, 2> d(0);
      d(0, 0) = 1;
      d(1, 1) = 1;
      d.rotate(0, 1, -angleX * coef);
      geom::Vector<float, 2> vec(0, 0.001);
      vec *= d;
      static_cast<geom::Vector<float, 2>&>(camera.position) += vec;
    }
    if (keystates[SDL_SCANCODE_S]) {
      geom::Matrix<float, 2, 2> d(0);
      d(0, 0) = 1;
      d(1, 1) = 1;
      d.rotate(0, 1, -angleX * coef);
      geom::Vector<float, 2> vec(0, -0.001);
      vec *= d;
      static_cast<geom::Vector<float, 2>&>(camera.position) += vec;
    }
    if (keystates[SDL_SCANCODE_A]) {
      geom::Matrix<float, 2, 2> d(0);
      d(0, 0) = 1;
      d(1, 1) = 1;
      d.rotate(0, 1, -angleX * coef);
      geom::Vector<float, 2> vec(-0.001, 0);
      vec *= d;
      static_cast<geom::Vector<float, 2>&>(camera.position) += vec;
    }
    if (keystates[SDL_SCANCODE_D]) {
      geom::Matrix<float, 2, 2> d(0);
      d(0, 0) = 1;
      d(1, 1) = 1;
      d.rotate(0, 1, -angleX * coef);
      geom::Vector<float, 2> vec(0.001, 0);
      vec *= d;
      static_cast<geom::Vector<float, 2>&>(camera.position) += vec;
    }


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

    auto next_triangle2 = camera.apply(triangle2);
    surface.draw(
      camera, 
      next_triangle2, 
      eng3d::ContextOneColor(
        SDL_Color(0, 255, 0)
      )
    );

    auto next_triangle = camera.apply(triangle);
    surface.draw(
      camera, 
      next_triangle, 
      eng3d::ContextOneColor(
        SDL_Color(0, 255, 255)
      )
    );

    
    
   
    SDL_UpdateWindowSurface(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
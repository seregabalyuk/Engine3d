// #define DEBUG_STACKCONVEX
// #define NO_CLIPPING
#define DEBUG_TRAPEZOID

#include "../src/eng3d/SDL.h"
#include "../src/eng3d/Camera.h"
#include "../src/eng3d/ContextOneColor.h"

#include <iostream>
#include <vector>

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow(
    "cube",
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED,
    1920, 
    1080, 
    SDL_WINDOW_SHOWN
  );
  auto screen = SDL_GetWindowSurface(window);
  SDL_Event event;
  bool run = true;

  auto surface = eng3d::buildSurface(screen);
  

  geom::Vector<int, 2> mousePos(0, 0);
  SDL_GetMouseState(&mousePos.x, &mousePos.y);

  eng3d::Camera camera(1.f, 16.f / 9.f, 1.f);
  camera.matrix *= 10.f;

  camera.position = geom::Vector(-2.f, -2, 0);
  
  std::vector<geom::Triangle<float, 3>> cube;
  for (int i = 0; i < 3; ++ i) {
    geom::Vector<float, 3> vec[4];
    size_t j = 0;
    for (auto x: std::vector{-1, 1}) {
      for (auto y: std::vector{-1, 1}) {
        vec[j][i] = 1;
        vec[j][(i + 1) % 3] = x;
        vec[j][(i + 2) % 3] = y;
        ++ j;
      }
    }
    cube.emplace_back(vec[0], vec[2], vec[1]);
    cube.emplace_back(vec[1], vec[2], vec[3]);
    j = 0;
    for (auto x: std::vector{-1, 1}) {
      for (auto y: std::vector{-1, 1}) {
        vec[j][i] = -1;
        vec[j][(i + 1) % 3] = x;
        vec[j][(i + 2) % 3] = y;
        ++ j;
      }
    }
    cube.emplace_back(vec[0], vec[1], vec[2]);
    cube.emplace_back(vec[2], vec[1], vec[3]);
    
  }

  std::vector<geom::Triangle<float, 3>> buf;
  buf.resize(cube.size());


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
    if (angleY * coef > M_PI) {
      angleY = M_PI / coef;
    }
    if (angleY < 0) {
      angleY = 0;
    }
    
    camera.matrix = mat;
    //camera.matrix.transpose();
    camera.matrix.rotate(1, 2, -angleY * coef);
    camera.matrix.rotate(0, 1, -angleX * coef);
    camera.matrix.transpose();
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

    if (keystates[SDL_SCANCODE_SPACE]) {
      camera.position.z += 0.001;
    }

    if (keystates[SDL_SCANCODE_LSHIFT]) {
      camera.position.z -= 0.001;
    }


    SDL_FillRect(screen, NULL, 0);
    
    for (size_t i = 0; i < cube.size(); ++ i) {
      buf[i] = camera.apply(cube[i]);
    }
    geom::Vector light(1.f, 0.5f, -1.7f);
    light.normalize();
    for (size_t i = 0; i < cube.size(); ++ i) {
      auto normal = geom::cross(
        cube[i].A - cube[i].B, 
        cube[i].C - cube[i].B
      );
      normal.normalize();

      float d = geom::dot(light, normal);
      int color = std::max(0.f, d) * 127 + 128;
      surface.draw(
        camera,
        buf[i],
        eng3d::ContextOneColor(
          SDL_Color(color, 0, 0)
        )
      );
    }

    
    SDL_UpdateWindowSurface(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
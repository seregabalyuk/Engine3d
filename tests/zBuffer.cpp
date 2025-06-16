// #define DEBUG_STACKCONVEX
// #define NO_CLIPPING
// #define DEBUG_TRAPEZOID

#include "../src/eng3d/SDL.h"
#include "../src/eng3d/Camera.h"
#include "../src/eng3d/ContextOneColor.h"
#include "../src/eng3d/LoadOBJ.h"


#include <iostream>
#include <vector>

int main() {
  std::string filename = "models/monkey.obj";
  // std::cin >> filename;

  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow(
    "zBuffer",
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED,
    1920, 
    1080, 
    SDL_WINDOW_SHOWN
  );
  auto screen = SDL_GetWindowSurface(window);
  SDL_Event event;
  bool run = true;

  auto surface = eng3d::buildSurfaceZbuffer<float>(screen);
  

  geom::Vector<int, 2> mousePos(0, 0);
  SDL_GetMouseState(&mousePos.x, &mousePos.y);

  eng3d::Camera camera(1.f, 16.f / 9.f, 1.f);
  camera.matrix *= 10.f;

  camera.position = geom::Vector(-2.f, -2, 0);
  
  

  auto obj = eng3d::loadOBJ<float>(filename);
  auto objbuf = obj;

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
  auto cubebuf = cube;


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
    float speed = 0.01;
    bool isZbuf = false;
    { // camera
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
        geom::Vector<float, 2> vec(0, speed);
        vec *= d;
        static_cast<geom::Vector<float, 2>&>(camera.position) += vec;
      }
      if (keystates[SDL_SCANCODE_S]) {
        geom::Matrix<float, 2, 2> d(0);
        d(0, 0) = 1;
        d(1, 1) = 1;
        d.rotate(0, 1, -angleX * coef);
        geom::Vector<float, 2> vec(0, -speed);
        vec *= d;
        static_cast<geom::Vector<float, 2>&>(camera.position) += vec;
      }
      if (keystates[SDL_SCANCODE_A]) {
        geom::Matrix<float, 2, 2> d(0);
        d(0, 0) = 1;
        d(1, 1) = 1;
        d.rotate(0, 1, -angleX * coef);
        geom::Vector<float, 2> vec(-speed, 0);
        vec *= d;
        static_cast<geom::Vector<float, 2>&>(camera.position) += vec;
      }
      if (keystates[SDL_SCANCODE_D]) {
        geom::Matrix<float, 2, 2> d(0);
        d(0, 0) = 1;
        d(1, 1) = 1;
        d.rotate(0, 1, -angleX * coef);
        geom::Vector<float, 2> vec(speed, 0);
        vec *= d;
        static_cast<geom::Vector<float, 2>&>(camera.position) += vec;
      }

      if (keystates[SDL_SCANCODE_SPACE]) {
        camera.position.z += speed;
      }

      if (keystates[SDL_SCANCODE_LSHIFT]) {
        camera.position.z -= speed;
      }

      if (keystates[SDL_SCANCODE_Z]){
        isZbuf = true;
      }
    }
    geom::Matrix<float, 3, 3> rotor(0);
    rotor(0, 0) = rotor(1, 1) = rotor(2, 2) = 1;
    rotor.rotate(0, 1, 0.001);
    //rotor.rotate(1, 2, 0.001);
    

    SDL_FillRect(screen, NULL, 0);

    surface.reset(10000);

   
    {
      for (size_t i = 0; i < obj.size(); ++ i) {
        obj[i] *= rotor;
        objbuf[i] = camera.apply(obj[i]);
      }

      geom::Vector light(1.f, 0.5f, -1.7f);
      light *= camera.matrix;
      light.normalize();
      for (size_t i = 0; i < objbuf.size(); ++ i) {
        auto normal = geom::cross(
          objbuf[i].A - objbuf[i].B, 
          objbuf[i].C - objbuf[i].B
        );
        normal.normalize();

        float d = geom::dot(light, normal);
        int color = std::max(0.f, d) * 127 + 128;
        surface.draw(
          camera,
          objbuf[i],
          eng3d::ContextOneColor(
            SDL_Color(color, 0, 0)
          )
        );
      }
     
        
    }
    if (isZbuf) {
      for (size_t i = 0; i < surface.width * surface.height; ++ i) {
        float d = 1.f / (surface.ptrZ()[i] / 3 + 1.f);
        
        surface.ptr()[i] = SDL_Color(255 * d, 255 * d, 255 * d);
      }
    }
    
    SDL_UpdateWindowSurface(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
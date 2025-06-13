#pragma once

#include <SDL2/SDL.h>

#include "Surface.h"

#define COLOR SDL_Color


namespace eng3d {
  Surface<SDL_Color> buildSurface(
    const SDL_Surface* surface
  ) {
    return Surface<SDL_Color>(
      static_cast<SDL_Color*>(surface->pixels), 
      surface->w, 
      surface->h
    );
  }
} // namespace eng3d
#pragma once 

#include "Surface.h"


namespace eng3d {
  template<class T, class Float>
  class SurfaceZbuffer: public Surface<T> {
    class Context {
      SurfaceZbuffer* surface;
      T* pixel_y;
      T* pixel_x;
      Float* zBuf;
     public:
      Context(SurfaceZbuffer* surface):
        surface(surface) {}
      
      void setY(size_t y) & {
        pixel_y = surface->ptr() + y * surface->width;
      }
      
      void setX(size_t x) & {
        pixel_x = pixel_y + x;
        size_t shift = pixel_x - surface->ptr();
        zBuf = surface->ptrZ() + shift;
      }

      void incY() & {
        pixel_y += surface->width;
      }

      void incX() & {
        ++ pixel_x;
        ++ zBuf;
      }
      
      void set(auto& context) & {
        auto z = context.getZ();
        //*zBuf = 0;
        if (z < *zBuf) {
          *pixel_x = context.getColor();
          *zBuf = z;
        }
      }
    }; 
   public:
    using Surface<T>::width;
    using Surface<T>::height;
    
    Float* zBuffer;

    SurfaceZbuffer(
      T* pixels, 
      size_t width, 
      size_t height
    ): Surface<T>(pixels, width, height) {
      zBuffer = new Float[width * height]();
    }

    Float* ptrZ() { return zBuffer; }
    const Float* ptrZ() const { return zBuffer; }

    void reset(const Float& val) {
      auto end = zBuffer + width * height;
      for (auto it = zBuffer; it < end; ++ it) {
        *it = val;
      }
    }

    Context context() {
      return Context(this);
    }

    template<class U, class ...Args>
    void draw(const U& obj, Args&&... args) {
      obj.draw(*this, std::forward<Args>(args) ...);
    }
  };
} // namespace eng3d
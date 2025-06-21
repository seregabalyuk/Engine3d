#pragma once

#include <utility>


namespace eng2d {
  template<class T>
  class Surface {
    class Context {
      Surface* _surface;
      T* _iterY;
      T* _iterX;
     public:
      Context(Surface* surface):
        _surface(surface) {}

      void setY(size_t y) & {
        _iterY = _surface->ptr() + y * _surface->width;
      }
      
      void setX(size_t x) & {
        _iterX = _iterY + x;
      }

      void incY() & {
        _iterY += _surface->width;
      }

      void incX() & {
        ++ _iterX;
      }
      
      void set(auto&& context) & {
        *_iterX = context.getColor();
      }
    };
   public:
   // usings
    using Pixel = T;
   // members
    size_t width;
    size_t height;

    T* pixels;
   // constructors
    Surface(T* pixels, size_t width, size_t height):
      pixels(pixels),
      width(width),
      height(height)
    {}

    T& operator()(size_t x, size_t y) {
      return pixels[x + y * width];
    }

    const T& operator()(size_t x, size_t y) const {
      return pixels[x + y * width];
    }

    T* ptr(size_t x, size_t y) {
      return pixels + (x + y * width);
    }

    T* ptr() { return pixels; }

    template<class U, class ...Args>
    void draw(const U& obj, Args&&... args) {
      obj.draw(*this, std::forward<Args>(args) ...);
    }

    Context context() {
      return Context(this);
    }
  };
} // namespace d3
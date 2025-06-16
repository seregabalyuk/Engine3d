#pragma once
#include <utility>


namespace eng3d {
  template<class T>
  class Surface {
    class Context {
      Surface* surface;
      T* iter_y;
      T* iter_x;
     public:
      Context(Surface* surface):
        surface(surface) {}

      void setY(size_t y) & {
        iter_y = surface->ptr() + y * surface->width;
      }
      
      void setX(size_t x) & {
        iter_x = iter_y + x;
      }

      void incY() & {
        iter_y += surface->width;
      }

      void incX() & {
        ++ iter_x;
      }
      
      void set(auto& context) & {
        *iter_x = context.getColor();
      }
    };
   public:
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
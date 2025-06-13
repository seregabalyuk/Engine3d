#pragma once
#include <utility>


namespace eng3d {
  template<class T>
  struct Surface {
   // members
    int width;
    int height;

    T* pixels;
   // constructors
    Surface(T* pixels, size_t width, size_t height):
      pixels(pixels),
      width(width),
      height(height)
    {}

    T& operator()(int x, int y) {
      return pixels[x + y * width];
    }

    const T& operator()(int x, int y) const {
      return pixels[x + y * width];
    }

    template<class U, class ...Args>
    void draw(const U& obj, Args&&... args) {
      obj.draw(*this, std::forward<Args>(args) ...);
    }
  };
} // namespace d3
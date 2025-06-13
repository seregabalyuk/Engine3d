#pragma once

namespace eng3d {
  template<class Color>
  struct ContextOneColor {
    Color color;
    Color getColor() { return color; }
    void setX(int x) {};
    void setY(int x) {};
    void incX() {};
    void incY() {};
  };

} // namespace eng3d
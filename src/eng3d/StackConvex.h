#pragma once
#include "Trapezoid.h"
#include <initializer_list>


namespace eng3d {
  template<class T, size_t N>
  struct StackConvex: StackConvex<T, N - 1> {
    using Point = geom::Vector<T, 2>;
    using StackConvex<T, N - 1>::begin;
    Point back;

    StackConvex() = default;

    StackConvex(std::initializer_list<Point> points) {
      auto it = points.begin();
      for (int i = 0; i < N; ++ i) {
        begin()[i] = *it;
        ++ it;
      }
    }

    void normalize() {
      auto it_min = begin();
      for (auto& point: *this) {
        if (point.y < it_min->y) {
          it_min = &point;
        }
      }
      ++ it_min;
      std::reverse(it_min, end());
      std::reverse(begin(), it_min);
      // Need delete
      if (N > 3) {
        auto& A = begin()[0];
        auto& B = begin()[1];
        auto& C = begin()[2];
        if (cross(B - A, C - A) < 0) {
          std::reverse(begin() + 1, end());
        }
      }
    }

    Point* end() {
      return begin() + N;
    }

    const Point* end() const {
      return begin() + N;
    }

    const Point* cend() const {
      return begin() + N;
    }

    static constexpr size_t size() { return N; }


    template<class Pixel, class ...Args>
    void draw(
      Surface<Pixel>& surface,
      Args&&... args
    ) const {
      auto it_min = begin();
      auto it_max = begin();
      { // find it_max
        auto x_min = it_min->x;
        auto x_max = it_min->x;
        for (auto& point: *this) {
          if (point.y > it_max->y) {
            it_max = &point;
          }
          x_min = std::min(point.x, x_min);
          x_max = std::max(point.x, x_max);
        }
        if (x_max <= 0) {
          return;
        }
        if (x_min >= surface.width) {
          return;
        }
        if (it_max->y <= 0) {
          return;
        }
        if (it_min->y >= surface.height) {
          return;
        }
      }
      geom::Line left(it_min[N-1], it_min[0]);
      geom::Line right(it_min[1], it_min[0]);
      auto y_from = it_min->y;
      auto it_left = it_min + (N - 1);
      auto it_right = it_min + 1;
      

      while(it_right <= it_max) {
        T y_to;
        if (it_left->y < it_right->y) {
          y_to = it_left->y;
          Trapezoid trap(y_from, y_to, left, right);
          trap.draw(surface, std::forward<Args>(args) ...);
          -- it_left;
          left = geom::Line(it_left[0], it_left[1]);
        } else {
          y_to = it_right->y;
          Trapezoid trap(y_from, y_to, left, right);
          trap.draw(surface, std::forward<Args>(args) ...);
          right = geom::Line(it_right[1], it_right[0]);
          ++ it_right;
        }
        y_from = y_to;
      }
      #ifdef DEBUG_STACKCONVEX
        int h = 2;
        for (auto& point: *this) {
          int x_from = std::max((int)point.x - h, 0);
          int x_to = std::min((int)point.x + h, surface.width);
          int y_from = std::max((int)point.y - h, 0);
          int y_to = std::min((int)point.y + h, surface.height);
          for (int y = y_from; y < y_to; ++ y)
          for (int x = x_from; x < x_to; ++ x)
            surface(x, y) = COLOR(0, 0, 255);
        }
      #endif
    }

    StackConvex& operator += (
      const Point& move
    ) & {
      for (auto& point: (*this)) {
        point += move;
      }
      return *this;
    }

    StackConvex& operator -= (
      const Point& move
    ) & {
      for (auto& point: (*this)) {
        point -= move;
      }
      return *this;
    }
  };
} // namespace eng3d


namespace eng3d {
  template<class T>
  struct StackConvex<T, 0> {
    using Point = geom::Vector<T, 2>;

    template<class ...Args>
    StackConvex(Args&&... args) {}

    Point* begin() {
      return reinterpret_cast<Point*>(this);
    }

    const Point* begin() const {
      return reinterpret_cast<const Point*>(this);
    }

    const Point* cbegin() const {
      return reinterpret_cast<const Point*>(this);
    }

    Point& operator [](size_t i) {
      return begin()[i];
    }

    const Point& operator [](size_t i) const {
      return begin()[i];
    }
  };
} // namespace eng3d
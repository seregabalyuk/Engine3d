#pragma once

#include "Vector.h"

namespace geom {
  template<class T, size_t N>
  struct Line;
} // namespace geom


namespace geom { // with 2 dimentions
  template<class T>
  struct Line<T, 2> {
    T A; // x coef
    T B; // y coef
    T C; // free coef
    // A x + B y = C

    using Point = Vector<T, 2>;

    Line(const Point& p1, const Point& p2):
      A(p1.y - p2.y),
      B(p2.x - p1.x),
      C(p2.x * p1.y - p1.x * p2.y)
    {}

    Line(
      const T& A, 
      const T& B, 
      const T& C
    ): A(A), B(B), C(C)
    {}

    void normalize() {
      if (A < 0) {
        A *= -1;
        B *= -1;
        C *= -1;
      }
    }

    T x(const T& y) const {
      return (C - B * y) / A;
    }

    T y(const T& x) const {
      return (C - A * x) / B;
    }

    Line& operator += (const Point& move) {
      C += A * move.x + B* move.y;
      return *this;
    }

    Line& operator -= (const Point& move) {
      C -= A * move.x + B* move.y;
      return *this;
    }

    Line& operator = (const Line&) & = default;
    Line& operator = (Line&&) & = default;
  };
} // namespace geom


namespace geom { // help compiler
  template<class T, size_t N>
  Line(
    const Vector<T, N>&,
    const Vector<T, N>&
  ) -> Line<T, N>;
} // namespace geom
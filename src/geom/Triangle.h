#pragma once
#include "Vector.h"

namespace geom {
  template<class T, size_t N>
  struct Triangle {
    using Point =  Vector<T, N>;
    
    union {
      Point points[3];
      struct {
        Point A, B, C;
      };
    };
    Triangle(): A(), B(), C() {};
    Triangle(
      const Point& A,
      const Point& B,
      const Point& C
    ): A(A), B(B), C(C) {}

    Triangle(
      const Triangle& other
    ):A(other.A), 
      B(other.B), 
      C(other.C) 
    {}

    ~Triangle() {
      A.~Point();
      B.~Point();
      C.~Point();
    }

    Triangle& operator =(const Triangle& other) & {
      A = other.A;
      B = other.B;
      C = other.C;
      return *this;
    }

    Triangle& operator =(Triangle&& other) & {
      A = std::move(other.A);
      B = std::move(other.B);
      C = std::move(other.C);
      return *this;
    }

    template <class Con>
    Triangle& operator*=(
      const Matrix<T, N, N, Con>& matrix  
    ) {
      A *= matrix;
      B *= matrix;
      C *= matrix;
      return *this;
    }

    template <class Con>
    Triangle& rev_mul(
      const Matrix<T, N, N, Con>& matrix  
    ) {
      A.rev_mul(matrix);
      B.rev_mul(matrix);
      C.rev_mul(matrix);
      return *this;
    }

    Triangle& operator += (
      const Point& point
    ) {
      A += point;
      B += point;
      C += point;
      return *this;
    }

    Triangle& operator -= (
      const Point& point
    ) {
      A -= point;
      B -= point;
      C -= point;
      return *this;
    }

    Point& operator [](size_t i) {
      return points[i];
    }

    const Point& operator [](size_t i) const  {
      return points[i];
    }
  };
} // namespace geom 


namespace geom {
  template<class T, size_t N, size_t M, class Con>
  Triangle<T, M> operator*(
    const Triangle<T, N>& triangle,
    const Matrix<T, N, M, Con>& matrix
  ) {
    return Triangle<T, M>(
      triangle.A * matrix,
      triangle.B * matrix,
      triangle.C * matrix
    );
  }

  template<class T, size_t N, size_t M, class Con>
  Triangle<T, M> operator*(
    const Matrix<T, N, M, Con>& matrix,
    const Triangle<T, N>& triangle
  ) {
    return Triangle<T, M>(
      matrix * triangle.A,
      matrix * triangle.B,
      matrix * triangle.C
    );
  }
} // namespace geom 
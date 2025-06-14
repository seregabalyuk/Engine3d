#pragma once

#include "matrix/DefaultContainer.h"
#include "matrix/LinkContainer.h"

#include <utility>
#include <cmath>

namespace geom {
  template<
    class T, 
    size_t N, 
    size_t M, 
    class Container = 
      matrix::DefaultContainer<T, N, M>
  > struct Matrix: Container {
    using Container::Container;
    using Container::operator();

    template<class CO>
    Matrix& operator = (
      const Matrix<T, N, M, CO>& other
    ) & {
      for (size_t i = 0; i < N; ++ i) {
        for (size_t j = 0; j < M; ++ j) {
          (*this)(i, j) = other(i, j);
        }
      }
      return *this;
    }

    template<class CO>
    Matrix& operator = (
      Matrix<T, N, M, CO>&& other
    ) & {
      for (size_t i = 0; i < N; ++ i) {
        for (size_t j = 0; j < M; ++ j) {
          (*this)(i, j) = std::move(other(i, j));
        }
      }
      return *this;
    }

    template<class CO>
    Matrix& operator += (
      const Matrix<T, N, M, CO>& other
    ) & {
      for (size_t i = 0; i < N; ++ i) {
        for (size_t j = 0; j < M; ++ j) {
          (*this)(i, j) += other(i, j);
        }
      }
      return *this;
    }

    template<class CO>
    Matrix& operator -= (
      const Matrix<T, N, M, CO>& other
    ) & {
      for (size_t i = 0; i < N; ++ i) {
        for (size_t j = 0; j < M; ++ j) {
          (*this)(i, j) -= other(i, j);
        }
      }
      return *this;
    }

    Matrix& operator *= (
      const T& scalar
    ) & {
      for (size_t i = 0; i < N; ++ i) {
        for (size_t j = 0; j < M; ++ j) {
          (*this)(i, j) *= scalar;
        }
      }
      return *this;
    }

    Matrix& rev_mul (
      const T& scalar
    ) & {
      for (size_t i = 0; i < N; ++ i) {
        for (size_t j = 0; j < M; ++ j) {
          (*this)(i, j) = scalar * (*this)(i, j);
        }
      }
      return *this;
    }

    Matrix& operator /= (
      const T& scalar
    ) & {
      for (size_t i = 0; i < N; ++ i) {
        for (size_t j = 0; j < M; ++ j) {
          (*this)(i, j) /= scalar;
        }
      }
      return *this;
    }

    template<class CO>
    Matrix& operator *= (
      const Matrix<T, M, M, CO>& other
    ) & {
      if (
        static_cast<const void*>(&other) == 
        static_cast<void*>(this)
      ) {
        return (*this) = (*this) * other;
      }
      char buffer[M * sizeof(T)];
      for (size_t i = 0; i < N; ++ i) {
        T* arr = reinterpret_cast<T*>(buffer);

        for (size_t j = 0; j < M; ++ j) {
          new(arr + j) T((*this)(i, 0) * other(0, j));
          for (size_t k = 1; k < M; ++ k) {
            arr[j] += (*this)(i, k) * other(k, j);
          }
        }
        int j = 0;
        for (
          auto it = arr; 
          it < arr + M;
          ++ it, ++ j
        ) {
          (*this)(i, j) = *it;
          it->~T();
        }
      }
      return *this;
    }
   // function
    Matrix& transpose() & {
      static_assert(N == M, "matrix is not square");
      for (size_t i = 0; i < N; ++ i) {
        for (size_t j = i + 1; j < M; ++ j) {
          std::swap((*this)(i, j), (*this)(j, i));
        }
      }
      return *this;
    }
    
    Matrix& rotate(
      size_t AxisX,
      size_t AxisY,
      const T& angle
    ) & {
      for (size_t i = 0; i < N; ++ i) {
        T x = (*this)(i, AxisX);
        T y = (*this)(i, AxisY);
        T sin = std::sin(angle);
        T cos = std::cos(angle);
        (*this)(i, AxisX) = x * cos - y * sin;
        (*this)(i, AxisY) = x * sin + y * cos;
      }
      return *this;
    }
  };
} // namespace geom


namespace geom { // operators
  template<
    class T, 
    size_t N,
    size_t M,
    size_t K,
    class C1,
    class C2
  > auto operator *(
    const Matrix<T, N, M, C1>& left,
    const Matrix<T, M, K, C2>& right
  ) {
    Matrix<
      T, N, K,
      matrix::DefaultContainer<T, N, K>
    > ret;
    for (size_t i = 0; i < N; ++ i) {
      for (size_t k = 0; k < K; ++ k) {
        ret(i, k) = left(i, 0) * right(0, k);
        for (size_t j = 1; j < M; ++ j) {
          ret(i, k) += left(i, j) * right(j, k);
        }
      }
    }
    return ret;
  }

  template<
    class T, 
    size_t N,
    size_t M,
    class C1,
    class C2
  > auto operator +(
    const Matrix<T, N, M, C1>& left,
    const Matrix<T, N, M, C2>& right
  ) {
    Matrix<
      T, N, M,
      matrix::DefaultContainer<T, N, M>
    > ret;
    ret = left;
    return ret += right;
  }

  template<
    class T, 
    size_t N,
    size_t M,
    class C1,
    class C2
  > auto operator -(
    const Matrix<T, N, M, C1>& left,
    const Matrix<T, N, M, C2>& right
  ) {
    Matrix<
      T, N, M,
      matrix::DefaultContainer<T, N, M>
    > ret;
    ret = left;
    return ret -= right;
  }

  template<
    class T, 
    size_t N,
    size_t M,
    class C
  > auto operator *(
    const Matrix<T, N, M, C>& left,
    const T& right
  ) {
    Matrix<
      T, N, M,
      matrix::DefaultContainer<T, N, M>
    > ret;
    ret = left;
    return ret *= right;
  }

  template<
    class T, 
    size_t N,
    size_t M,
    class C
  > auto operator *(
    const T& left,
    const Matrix<T, N, M, C>& right
  ) {
    Matrix<
      T, N, M,
      matrix::DefaultContainer<T, N, M>
    > ret;
    ret = right;
    return ret.rev_mul(left);
  }

  template<
    class T, 
    size_t N,
    size_t M,
    class C
  > auto operator /(
    const Matrix<T, N, M, C>& left,
    const T& right
  ) {
    Matrix<
      T, N, M,
      matrix::DefaultContainer<T, N, M>
    > ret;
    ret = left;
    return ret /= right;
  }
} // namespace geom


namespace geom { // functions
  template<class T, class Con>
  auto det(const Matrix<T, 3, 3, Con>& m) {
    return 
      m(0, 0) * m(1, 1) * m(2, 2) +
      m(0, 1) * m(1, 2) * m(2, 0) +
      m(0, 2) * m(1, 0) * m(2, 1) -
      m(0, 0) * m(1, 2) * m(2, 1) -
      m(0, 1) * m(1, 0) * m(2, 2) -
      m(0, 2) * m(1, 1) * m(2, 0);
  }

  template<class T, class Con>
  auto det(const Matrix<T, 2, 2, Con>& m) {
    return 
      m(0, 0) * m(1, 1)  -
      m(0, 1) * m(1, 0);
  }
} // namespace geom


namespace geom {
  template<class Con>
  auto WrapMatrix(Con& con) {
    using LinkCon = matrix::LinkContainer<Con, 0>;
    return Matrix<
      typename LinkCon::Type,
      LinkCon::N,
      LinkCon::M,
      LinkCon
    >(con);
  }

  template<class Con>
  auto WrapMatrix(const Con& con) {
    using LinkCon = matrix::LinkContainer<Con, 1>;
    return Matrix<
      typename LinkCon::Type,
      LinkCon::N,
      LinkCon::M,
      LinkCon
    >(con);
  }
} // namespace
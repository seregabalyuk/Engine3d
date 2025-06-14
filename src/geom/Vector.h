#pragma once

#include "Axis.h"
#include "../traits/GetFromPack.h"
#include "../math/Zero.h"
#include "Matrix.h"

#include <type_traits>


namespace geom { // Vector<T, N>
  template<class T, size_t N>
  struct Vector: Vector<T, N - 1>, Axis<T, N> {
   private:
    using _Axis = Axis<T, N>;
    using _Parant = Vector<T, N - 1>;
    
   public:
    using Axis<T, N>::back;
    using value_type = T;
    using Vector<T, N - 1>::begin;
    using Vector<T, N - 1>::cbegin;
   // constructors
    Vector(const Vector& other): 
      _Parant(other), 
      _Axis(other.back)
    {}

    Vector(Vector&& other): 
      _Parant(std::move(other)), 
      _Axis(std::move(other.back))
    {}

    template<class U, class Func>
    Vector(
      const Vector<U, N>& vec, 
      Func func
    ):_Parant(vec, func),
     _Axis(Func::function(vec.back)) {}

    Vector() {}

    template<class... Args>
    Vector(const T& val,Args&&... args):
      _Parant(
        val, 
        std::forward<Args>(args)...
      ),
      _Axis(traits::get<N - 1>(
        val, 
        std::forward<Args>(args)...
      ))
    {}

    template<class... Args>
    Vector(T&& val,Args&&... args):
      _Parant(
        std::move(val), 
        std::forward<Args>(args)...
      ),
      _Axis(traits::get<N - 1>(
        std::move(val),
        std::forward<Args>(args)...
      ))
    {}

    
   // operators =
    Vector& operator +=(const Vector& other) & {
      back += other.back;
      parant() += other;
      return *this;
    }

    Vector& operator -=(const Vector& other) & {
      back -= other.back;
      parant() -= other;
      return *this;
    }

    Vector& operator *=(const T& scalar) & {
      back *= scalar;
      parant() *= scalar;
      return *this;
    }

    template<class Con>
    Vector& operator *=(
      const Matrix<T, N, N, Con>& matrix
    ) & {
      (*this) = (*this) * matrix;
      return *this;
    }

    template<class Con>
    Vector& rev_mul(
      const Matrix<T, N, N, Con>& matrix
    ) & {
      (*this) = matrix * (*this);
      return *this;
    }

    Vector& rev_mul(const T& scalar) & {
      back = scalar * back;
      parant().rev_mul(scalar);
      return *this;
    }

    Vector& operator /=(const T& scalar) & {
      back /= scalar;
      parant() /= scalar;
      return *this;
    }
  
    Vector& operator =(const Vector& other) & {
      back = other.back;
      parant() = other;
      return *this;
    }

    Vector& operator =(Vector&& other) & {
      back = std::move(other.back);
      parant() = std::move(other);
      return *this;
    }
    
   // functions
    static constexpr size_t size() { return N; }

    T* end() { return begin() + size(); }
    const T* end() const { return begin() + size(); }
    const T* cend() const { return cbegin() + size(); }
    
    _Parant& parant() & {
      return *this;
    }

    const _Parant& parant() const & {
      return *this;
    }

    auto sqrlength() const {
      return dot(*this, *this);
    }
  };
} // namespace geom


namespace geom { // Vector<T, 0>
  template<class T>
  struct Vector<T, 0> {
    template<class... Args>
    Vector(Args&& ...) {}

    Vector operator +=(const Vector&) & { return *this; }
    Vector operator -=(const Vector&) & { return *this; }
    Vector operator *=(const T&) & { return *this; }
    Vector operator /=(const T&) & { return *this; }
    Vector operator =(const Vector&) & { return *this; }
    Vector operator =(Vector&&) & { return *this; }


    T& operator[] (size_t id) {
      return begin()[id];
    }
    const T& operator[] (size_t id) const {
      return begin()[id];
    }

    T* begin() { 
      return reinterpret_cast<T*>(this); 
    }

    const T* cbegin() const { 
      return reinterpret_cast<const T*>(this); 
    }

    const T* begin() const { 
      return reinterpret_cast<const T*>(this); 
    }

    auto sqrlength() const {
      return dot(*this, *this);
    }
  };
} // namespace geom


namespace geom { // operators +-*/
  template<class T, size_t N>
  Vector<T, N> operator + (
    const Vector<T, N>& left, 
    const Vector<T, N>& right
  ) {
    Vector<T, N> ret = left;
    return ret += right;
  }

  template<class T, size_t N>
  Vector<T, N> operator - (
    const Vector<T, N>& left, 
    const Vector<T, N>& right
  ) {
    Vector<T, N> ret = left;
    return ret -= right;
  }

  template<class T, size_t N>
  Vector<T, N> operator * (
    const Vector<T, N>& left, 
    const T& right
  ) {
    Vector<T, N> ret = left;
    return ret *= right;
  }

  template<class T, size_t N>
  Vector<T, N> operator * (
    const T& left, 
    const Vector<T, N>& right
  ) {
    Vector<T, N> ret = right;
    return ret.rev_mul(left);
  }

  template<class T, size_t N, size_t M, class Con>
  Vector<T, M> operator *(
    const Vector<T, N>& left,
    const Matrix<T, N, M, Con>& right
  ) {
    Vector<T, M> ret;
    for (size_t j = 0; j < M; ++ j) {
      ret[j] = left[0] * right(0, j);
      for (size_t i = 1; i < N; ++ i) {
        ret[j] += left[i] * right(i, j);
      }
    }
    return ret;
  }

  template<class T, size_t N, size_t M, class Con>
  Vector<T, N> operator *(
    const Matrix<T, N, M, Con>& left,
    const Vector<T, M>& right
  ) {
    Vector<T, N> ret;
    for (size_t j = 0; j < N; ++ j) {
      ret[j] = left(j, 0) * right[0];
      for (size_t i = 1; i < M; ++ i) {
        ret[j] += left(j, i) * right[i];
      }
    }
    return ret;
  }


  template<class T, size_t N>
  Vector<T, N> operator / (
    const Vector<T, N>& left, 
    const T& right
  ) {
    Vector<T, N> ret = left;
    return ret /= right;
  }
} // namespace geom


namespace geom { // helping compiler
  template<class T, class ...Args>
  Vector(T&&, Args&&...) 
    -> Vector<
      std::remove_cvref_t<T>,
      sizeof...(Args) + 1
  >;
} // namespace geom


namespace geom { // functions
  

  template<class T, class U, size_t N>
  Vector<T, N> to(const Vector<U, N>& vec) {
    struct Preob {
      static T function(const U& in) { return in; }
    };
    return Vector<T, N>(
      vec, 
      Preob()
    );
  }


  template<class T>
  math::Zero dot(
    const Vector<T, 0>& left, 
    const Vector<T, 0>& right
  ) {
    return  math::Zero{};
  }

  template<class T, size_t N>
  T dot(
    const Vector<T, N>& left, 
    const Vector<T, N>& right
  ) {
    return dot(left.parant(), right.parant()) 
      + left.back * right.back;
  }

  template<class T>
  T cross(
    const Vector<T, 2>& left, 
    const Vector<T, 2>& right
  ) {
    return left.x * right.y - left.y * right.x;
  }

  template<class T>
  auto cross(
    const Vector<T, 3>& left, 
    const Vector<T, 3>& right
  ) {
    return Vector<T, 3>(
      left.y * right.z - left.z * right.y,
      left.z * right.x - left.x * right.z,
      left.x * right.y - left.y * right.x
    );
  }
} // namespace geom


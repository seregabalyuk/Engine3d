#pragma once


namespace geom { // declaration 
  template<class T, size_t N, size_t M, class CO>
  class Matrix;

  template<class T, size_t N>
  class Vector;

  template<class T, size_t N>
  class Line;

  template<class T, size_t N>
  class Ray;

  template<class T, size_t N>
  class Box;
} // namespace geom


namespace geom { // usings 
  template<class T>
  using Vector2 = Vector<T, 2>;

  template<class T>
  using Vector3 = Vector<T, 3>;
  
  template<class T>
  using Vector4 = Vector<T, 4>;
} // namespace geom


namespace geom { // functions
  template<class T, class U, size_t N>
  constexpr Vector<T, N> to(const Vector<U, N>&);

  template<class T, size_t N, size_t M, class CO>
  constexpr auto det(const Matrix<T, N, M, CO>&);

  template<class Con>
  constexpr auto WrapMatrix(Con&);

  template<class Con>
  constexpr auto WrapMatrix(const Con& con);

  template<class T, class... Args>
  constexpr auto RowsWrapMatrix(T& val, Args&&... args);

  template<class T, class... Args>
  constexpr auto RowsWrapMatrix(const T& val, Args&&... args);
} // namespace geom 
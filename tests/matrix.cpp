#include "../src/geom/Matrix.h"
#include "../src/traits/ToConst.h"

#include <iostream>


template<class T, size_t N, size_t M, class C>
void print(const geom::Matrix<T, N, M, C>& m) {
  for (size_t i = 0; i < N; ++ i) {
    for (size_t j = 0; j < M; ++ j) {
      std::cout << m(i, j) << ' ';
    }
    std::cout << '\n';
  }
}

int main() {
  geom::Matrix<float, 3, 3> f(1);
  f(0, 0) = 10;
  f(1, 1) = 2;
  f(2, 2) = 2;
  f(0, 2) = 2;
  f(2, 0) = 2;
  print(f);
  std::cout << '\n';
  auto g = f * f;
  f *= f;
  print(g);
  std::cout << '\n';
  print(f);
  std::cout << '\n';
  
  std::cout << geom::det(f) << ' ' << geom::det(g) << '\n';

  int p[2][2] = {{3, 4}, {9, -1}};
  auto l = geom::WrapMatrix(p);
  l *= 3;
  std::cout << p[0][1] << '\n';

  int a1[2] = {1, 2};
  int a2[2] = {2, 3};
  auto dd = &a2;
  auto ff = geom::RowsWrapMatrix(a1, a2);
  std::cout << geom::det(ff) << '\n';
}
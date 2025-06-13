#include "../src/traits/ArrayStats.h"
#include "../src/geom/Vector.h"

#include <iostream>

int main() {
  int f[10];
  std::array<float, 12> g;
  geom::Vector u(1, 2, 3);
  std::cout << traits::ArrayStats<
    decltype(f)
  >::Size << '\n';
  std::cout << traits::ArrayStats<
    decltype(g)
  >::Size << '\n';
  typename traits::ArrayStats<
    decltype(u)
  >::Type pop;
  std::cout << typeid(traits::ArrayStats<
    decltype(u)
  >::Type).name() << '\n';

  std::array<float, 4> d[10];
  std::cout << traits::Array2DStats<
    decltype(d)
  >::N << '\n';
  std::cout << traits::Array2DStats<
    decltype(d)
  >::M << '\n';
}
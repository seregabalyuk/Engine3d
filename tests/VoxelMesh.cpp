#include "../src/geom/Vector.h"
#include "../src/eng3d/VoxelMesh.h"

#include <iostream>

int main() {
  constexpr geom::Vector<int, 3> f(1, 2, 3);
  constexpr geom::Vector<size_t, 3> h = geom::to<size_t>(f);
  
}
#pragma once
#include "../geom.h"
#include <vector>
#include <cstdint>

namespace eng3d {
  struct VoxelMesh {
    static constexpr geom::Vector<size_t, 3> count_div = {2, 2, 2};
    static constexpr size_t count_div_all = count_div.x * count_div.y * count_div.z;
    using Link = int64_t;
    using Real = float;
    
    struct Node {
      Link to[count_div_all];
      Node() {
        for (size_t i = 0; i < count_div_all; ++ i) {
          to[i] = 0;
        }
      }
    };

    geom::Vector<Real, 3> length;
    geom::Vector<Real, 3> position;

    std::vector<Node> nodes;
  };
}

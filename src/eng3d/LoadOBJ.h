#include "../geom/Triangle.h"

#include <fstream>
#include <vector>
#include <sstream>
#include <ranges>

namespace eng3d {
  template<class Float>
  auto loadOBJ(const std::string& filename) {
    std::vector<
      geom::Triangle<Float, 3>
    > triangles;
    std::vector<
      geom::Vector<Float, 3>
    > points;
    std::ifstream fin(filename);
    while(fin) {
      std::string line;
      std::getline(fin, line, '\n');
      std::stringstream ss(line);
      std::string name;
      ss >> name;
      if (name == "v") {
        points.emplace_back();
        ss >> points.back().x 
           >> points.back().y 
           >> points.back().z;
      } else if (name == "f") {
        int pr[3] = {0, 0, 0};
        for (int i = 0; i < 3; ++ i) {
          ss >> name;
          /*int v[3] = {0, 0, 0};
          int k = 0;
          std::string g;
          for (auto c: name){
            if (c == '/') {
              g = "";
            } else {
              g += c;
            }
            v[k ++] = std::stoi(g);
          }*/
          int v[1] = {std::stoi(name)};
          if (v[0] < 0) {
            v[0] = points.size() + v[0];
          } else {
            -- v[0];
          }
          pr[i] = v[0];
        }
        triangles.emplace_back(
          points[pr[0]],
          points[pr[1]],
          points[pr[2]]
        );
      }
    }
    return triangles;
  }
} // namespace eng3d
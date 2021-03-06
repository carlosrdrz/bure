#pragma once

#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "types.h"

namespace bure {

typedef std::function<std::unordered_set<map_coords>(map_coords pos,
                                                     map_coords dst)>
    neighbors_fun;

class pathfinding {
 public:
  static std::vector<map_coords> a_star(map_coords start,
                                        map_coords destination,
                                        neighbors_fun nf);

 private:
  static std::vector<map_coords> a_star_reconstruct(
      std::unordered_map<map_coords, map_coords>& cameFrom,
      map_coords& current);
  static int heuristic_cost_estimate(map_coords start, map_coords destination);
  static map_coords lowest_fscore(std::unordered_map<map_coords, int>& fScore,
                                  std::unordered_set<map_coords>& set);
  static std::unordered_set<map_coords> neighbours(map_coords node,
                                                   map_coords dst);
};

}  // namespace bure

#include "pathfinding.h"

#include <cmath>
#include <limits>

namespace bure {

// https://en.wikipedia.org/wiki/A*_search_algorithm
std::vector<map_coords> pathfinding::a_star(map_coords start,
                                            map_coords destination) {
  std::unordered_set<map_coords> closedSet = {};
  std::unordered_set<map_coords> openSet = {start};
  std::unordered_map<map_coords, map_coords> cameFrom;
  std::unordered_map<map_coords, int> gScore;
  std::unordered_map<map_coords, int> fScore;

  gScore[start] = 0;
  fScore[start] = heuristic_cost_estimate(start, destination);

  while (!openSet.empty()) {
    auto current = lowest_fscore(fScore, openSet);

    if (current == destination) {
      return a_star_reconstruct(cameFrom, current);
    }

    auto currentOpenSet = openSet.find(current);
    if (currentOpenSet != openSet.end()) openSet.erase(currentOpenSet);
    closedSet.emplace(current);

    for (const auto& neighbor : neighbours(current)) {
      if (closedSet.find(neighbor) != closedSet.end()) continue;

      if (gScore.find(current) == gScore.end()) {
        gScore[current] = std::numeric_limits<int>::infinity();
      }

      if (gScore.find(neighbor) == gScore.end()) {
        gScore[neighbor] = std::numeric_limits<int>::infinity();
      }

      auto tentative_gScore = gScore[current] + 1;

      if (openSet.find(neighbor) == openSet.end()) {
        openSet.emplace(neighbor);
      } else if (tentative_gScore >= gScore[neighbor]) {
        continue;
      }

      cameFrom[neighbor] = current;
      gScore[neighbor] = tentative_gScore;
      fScore[neighbor] =
          gScore[neighbor] + heuristic_cost_estimate(neighbor, destination);
    }
  }
}

std::vector<map_coords> pathfinding::a_star_reconstruct(
    std::unordered_map<map_coords, map_coords>& cameFrom, map_coords& current) {
  std::vector<map_coords> totalPath = {current};

  while (cameFrom.find(current) != cameFrom.end()) {
    current = cameFrom[current];
    totalPath.emplace_back(current);
  }

  return totalPath;
}

int pathfinding::heuristic_cost_estimate(map_coords start,
                                         map_coords destination) {
  return abs(destination.x - start.x) + abs(destination.y - start.y);
}

map_coords pathfinding::lowest_fscore(
    std::unordered_map<map_coords, int>& fScore,
    std::unordered_set<map_coords>& set) {
  map_coords min = undefined_map_coords;
  int minScore = std::numeric_limits<int>::infinity();

  for (const auto& mc : set) {
    if (fScore.find(mc) == fScore.end()) {
      fScore[mc] = std::numeric_limits<int>::infinity();
    }

    if (min == undefined_map_coords || fScore[mc] <= minScore) {
      min = mc;
      minScore = fScore[mc];
    }
  }

  return min;
}

std::unordered_set<map_coords> pathfinding::neighbours(map_coords node) {
  std::unordered_set<map_coords> result;

  // TODO(carlosrdrs): check map limits here
  result.emplace(map_coords{node.x, node.y - 1});
  result.emplace(map_coords{node.x - 1, node.y});
  result.emplace(map_coords{node.x + 1, node.y});
  result.emplace(map_coords{node.x, node.y + 1});

  return result;
}

}  // namespace bure

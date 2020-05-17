#include "pathfinding.h"

#include <cmath>
#include <limits>

#include "engine.h"
#include "logger.h"

#define MAX_ITERATIONS 1000

namespace bure {

// https://en.wikipedia.org/wiki/A*_search_algorithm
std::vector<map_coords> pathfinding::a_star(map_coords start,
                                            map_coords destination,
                                            neighbors_fun nf) {
  std::unordered_set<map_coords> closedSet = {};
  std::unordered_set<map_coords> openSet = {start};
  std::unordered_map<map_coords, map_coords> cameFrom;
  std::unordered_map<map_coords, int> gScore;
  std::unordered_map<map_coords, int> fScore;

  gScore[start] = 0;
  fScore[start] = heuristic_cost_estimate(start, destination);

  auto iter = 0;

  while (!openSet.empty()) {
    if (iter++ > MAX_ITERATIONS) {
      bure::logger::error("max iterations reached in a_star()");
      exit(1);
    }

    auto current = lowest_fscore(fScore, openSet);

    if (current == destination) {
      return a_star_reconstruct(cameFrom, current);
    }

    auto currentOpenSet = openSet.find(current);
    openSet.erase(currentOpenSet);
    closedSet.emplace(current);

    for (const auto& neighbor : nf(current, destination)) {
      if (closedSet.find(neighbor) != closedSet.end()) continue;

      auto tentative_gScore = gScore[current] + 1;

      if (openSet.find(neighbor) == openSet.end()) {
        openSet.emplace(neighbor);
        gScore[neighbor] = tentative_gScore;
        fScore[neighbor] =
            gScore[neighbor] + heuristic_cost_estimate(neighbor, destination);
      } else if (tentative_gScore >= gScore[neighbor]) {
        continue;
      }

      cameFrom[neighbor] = current;
    }
  }

  return std::vector<map_coords>();
}

std::vector<map_coords> pathfinding::a_star_reconstruct(
    std::unordered_map<map_coords, map_coords>& cameFrom, map_coords& current) {
  std::vector<map_coords> totalPath = {current};

  while (cameFrom.find(current) != cameFrom.end()) {
    current = cameFrom[current];
    totalPath.emplace_back(current);
  }

  std::reverse(totalPath.begin(), totalPath.end());
  totalPath.erase(totalPath.begin());

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

}  // namespace bure

#pragma once

#include <string>

namespace bure {

struct camera {
  int x;
  int y;
  int width;
  int height;
};

struct world_coords {
  int x;
  int y;

  bool operator==(const world_coords& rhs) const {
    return (this->x == rhs.x && this->y == rhs.y);
  }
};

extern world_coords undefined_world_coords;
extern world_coords source_world_coords;

struct map_coords {
  int x;
  int y;

  bool operator==(const map_coords& rhs) const {
    return (this->x == rhs.x && this->y == rhs.y);
  }
};

extern map_coords undefined_map_coords;
extern map_coords source_map_coords;

struct layer {
  int id;
  std::string name;
  int* data;
  bool visible;
};

struct tileset {
  int id;
  std::string name;
  std::string file;
  int firstGid;
  int gridWidth;
  int gridHeight;
};

struct tile {
  std::string file;
  int srcX;
  int srcY;
  int srcWidth;
  int srcHeight;
};

}  // namespace bure

namespace std {
template <>
struct hash<bure::map_coords> {
  std::size_t operator()(const bure::map_coords& k) const {
    std::size_t h1 = k.x;
    std::size_t h2 = k.y;

    return ((h1 ^ (h2 << 1)) >> 1);
  }
};

template <>
struct hash<bure::world_coords> {
  std::size_t operator()(const bure::world_coords& k) const {
    std::size_t h1 = k.x;
    std::size_t h2 = k.y;

    return ((h1 ^ (h2 << 1)) >> 1);
  }
};
}  // namespace std

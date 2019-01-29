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

  bool operator==(const world_coords& rhs) {
    return (this->x == rhs.x && this->y == rhs.y);
  }
};

extern world_coords undefined_world_coords;
extern world_coords source_world_coords;

struct map_coords {
  int x;
  int y;

  bool operator==(const map_coords& rhs) {
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

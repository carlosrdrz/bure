#pragma once

#include "types.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace bure {

class game_map {
 public:
  game_map(std::string basePath, std::string file);
  ~game_map();

  void setScale(int scale);

  int getScale();
  int getWidth();
  int getHeight();
  int getTileWidth();
  int getTileHeight();

  world_coords mapToWorld(map_coords m);
  map_coords worldToMap(world_coords m);

  layer getLayer(int layerId);
  tile getTileData(int tileGid);

  // These dont really make sense here
  // or maybe they do but cant be coupled with how to actually
  // check if a tile is walkable
  bool canWalk(map_coords mc);
  bool anyEntityIn(map_coords mc);

 private:
  int _scale = 1;
  int _width, _height;
  int _tileWidth, _tileHeight;

  std::vector<layer> _layers;
  std::vector<tileset> _tilesets;
  std::unordered_map<int, tile> _tiles;
};

}  // namespace bure

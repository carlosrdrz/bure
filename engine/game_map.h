#pragma once

#include "types.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace bure {

class game_map {
 public:
  game_map(int w, int h, int tw, int th)
      : _width(w), _height(h), _tileWidth(tw), _tileHeight(th) {}
  ~game_map();

  void setScale(int scale);

  int getScale();
  int getWidth();
  int getHeight();
  int getTileWidth();
  int getTileHeight();

  world_coords mapToWorld(map_coords m);
  map_coords worldToMap(world_coords m);
  bool isWithinLimits(map_coords m);

  layer getLayer(int layerId);
  void addLayer(layer l);
  void addTileset(tileset t);

  tile getTileData(int tileGid);

 private:
  int _scale = 1;
  int _width, _height;
  int _tileWidth, _tileHeight;

  std::vector<layer> _layers;
  std::vector<tileset> _tilesets;
  std::unordered_map<int, tile> _tiles;
};

}  // namespace bure

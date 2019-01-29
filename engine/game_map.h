#pragma once

#include <vector>
#include <string>
#include <unordered_map>

namespace bure {

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

struct map_coords {
  int x;
  int y;
};

struct world_coords {
  int x;
  int y;
};

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

  world_coords mapToScreen(map_coords m);
  map_coords screenToMap(world_coords m);

  layer getLayer(int layerId);
  tile getTileData(int tileGid);

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

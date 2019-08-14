#include "engine.h"
#include "game_map.h"
#include "utils/logger.h"
#include "components/solid_component.h"

#include <stdio.h>

namespace bure {

game_map::~game_map() {
  for (auto& layer : _layers) {
    delete layer.data;
  }
}

void game_map::setScale(float scale) { _scale = scale; }
float game_map::getScale() { return _scale; }

int game_map::getWidth() { return _width; }
int game_map::getHeight() { return _height; }
int game_map::getTileWidth() { return _tileWidth * _scale; }
int game_map::getTileHeight() { return _tileHeight * _scale; }

layer game_map::getLayer(int layerId) {
  return _layers.at(layerId);
}

tile game_map::getTileData(int tileGid) {
  if (_tiles.count(tileGid) > 0) return _tiles[tileGid];

  tile t;
  tileset ts;
  for (auto tst : _tilesets) {
    if (tileGid >= tst.firstGid) {
      ts = tst;
    }
  }

  t.file = ts.file;
  t.srcX = 0;
  t.srcY = 0;
  t.srcWidth = _tileWidth;
  t.srcHeight = _tileHeight;

  auto tileTsId = tileGid - ts.firstGid + 1;
  while (tileTsId > ts.gridWidth) {
    tileTsId -= ts.gridWidth;
    t.srcY += _tileHeight;
  }

  t.srcX = (tileTsId - 1) * _tileWidth;

  _tiles[tileGid] = t;
  return t;
}

world_coords game_map::mapToWorld(map_coords m) {
  return {
    m.x * getTileWidth(),
    m.y * getTileHeight()
  };
}

map_coords game_map::worldToMap(world_coords s) {
  return {
    s.x / getTileWidth(),
    s.y / getTileHeight()
  };
}

bool game_map::isWithinLimits(map_coords m) {
  return m.x >= 0 && m.x < getWidth() && m.y >= 0 && m.y < getHeight();
}

void game_map::addLayer(layer l) {
    _layers.push_back(l);
}

void game_map::addTileset(tileset t) {
  _tilesets.push_back(t);
}

void game_map::printMap(int layerId) {
  auto layer = getLayer(layerId);

  for (int i = 0; i < getHeight(); i++) {
    for (int j = 0; j < getWidth(); j++) {
      printf("%02d ", layer.data[i * getHeight() + j]);
    }

    printf("\n");
  }
}

}  // namespace bure

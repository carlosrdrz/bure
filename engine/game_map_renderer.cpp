#include "game_map_renderer.h"

#include "engine.h"
#include "utils/logger.h"

namespace bure {

void game_map_renderer::render(int layer) {
  auto map = bure::engine::get().getMap();
  if (map == nullptr) return;

  int jug_y = bure::engine::get().globalY;
  int jug_x = bure::engine::get().globalX;
  int abs_x = 0;
  int abs_y = 0;
  int x, y;

  for (y = (jug_y - 11); y < (jug_y + 12); y++) {
    for (x = (jug_x - 20); x < (jug_x + 20); x++) {
      if (x >= 0 && y >= 0 && x < map->getWidth() && y < map->getHeight()) {
        auto mapLayer = map->getLayer(layer);
        auto tileGid = mapLayer.data[(x + (y * map->getWidth()))];

        if (tileGid != 0 && mapLayer.visible) {
          renderTile(map->getTileData(tileGid), abs_x, abs_y,
                     map->getTileWidth(), map->getTileHeight());
        }
      }
      abs_x += map->getTileWidth();
    }

    abs_x = 0;
    abs_y += map->getTileHeight();
  }
}

void game_map_renderer::renderTile(tile t, int x, int y, int width, int height) {
  rect src, dst;

  src.x = t.srcX;
  src.y = t.srcY;
  src.width = width;
  src.height = height;

  dst.x = x;
  dst.y = y;
  dst.width = width;
  dst.height = height;

  _graphics->drawSprite(t.file, src, dst);
}

}  // namespace bure

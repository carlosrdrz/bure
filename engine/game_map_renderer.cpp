#include "game_map_renderer.h"

#include "engine.h"
#include "utils/logger.h"

namespace bure {

void game_map_renderer::render(int layer) {
  auto map = bure::engine::get().getMap();
  if (map == nullptr) return;

  auto camera = bure::engine::get().getCamera();
  int first_y = camera.y / map->getTileHeight();
  int first_x = camera.x / map->getTileWidth();
  int last_y = (camera.y + camera.height) / map->getTileHeight();
  int last_x = (camera.x + camera.width) / map->getTileWidth();
  int abs_y = (camera.y % map->getTileHeight()) * -1;
  int abs_x = (camera.x % map->getTileWidth()) * -1;
  auto mapLayer = map->getLayer(layer);

  for (int y = first_y; y <= last_y; y++) {
    for (int x = first_x; x <= last_x; x++) {
      auto tileGid = mapLayer.data[(x + (y * map->getWidth()))];

      if (tileGid != 0 && mapLayer.visible) {
        renderTile(map->getTileData(tileGid), abs_x, abs_y,
                   map->getTileWidth(), map->getTileHeight());
      }

      abs_x += map->getTileWidth();
    }

    abs_x = (camera.x % map->getTileWidth()) * -1;
    abs_y += map->getTileHeight();
  }
}

void game_map_renderer::renderTile(tile t, int x, int y, int width, int height) {
  rect src, dst;

  src.x = t.srcX;
  src.y = t.srcY;
  src.width = t.srcWidth;
  src.height = t.srcHeight;

  dst.x = x;
  dst.y = y;
  dst.width = width;
  dst.height = height;

  _graphics->drawSprite(t.file, src, dst);
}

}  // namespace bure

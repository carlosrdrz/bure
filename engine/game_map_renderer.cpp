#include "game_map_renderer.h"

#include "engine.h"
#include "utils/logger.h"

#define TILE_SIZE 32

namespace bure {

void game_map_renderer::render(int layer) {
  auto map = bure::engine::get().getMap();
  if (map == nullptr) return;

  int jug_y = bure::engine::get().globalY;
  int jug_x = bure::engine::get().globalX;
  int empezar_x, empezar_y = 0;
  int x, y;

  for (y = (jug_y - 12); y < (jug_y + 12); y++) {
    for (x = (jug_x - 16); x < (jug_x + 16); x++) {
      if (x >= 0 && y >= 0 && x < map->width && y < map->height) {
        if (layer == 0) {
          renderTile(map->mapa[(x + (y * map->width))], empezar_x, empezar_y, layer);
        } else {
          renderTile(map->elementos[(x + (y * map->width))], empezar_x, empezar_y, layer);
        }
      }
      empezar_x += TILE_SIZE;
    }
    empezar_x = 0;
    empezar_y += TILE_SIZE;
  }
}

void game_map_renderer::renderTile(int tile, int x, int y, int layer) {
  rect src, dst;

  int tilex = 0;
  int tiley = 0;

  if (layer == 0) {
    while (tile > 7) {
      tile -= 7;
      tiley += 32;
    }
    tilex = (tile - 1) * 32;
  } else {
    while (tile > 8) {
      tile -= 8;
      tiley += 32;
    }
    tilex = (tile - 1) * 32;
  }
  src.x = tilex;
  src.y = tiley;
  src.width = TILE_SIZE;
  src.height = TILE_SIZE;

  dst.x = x;
  dst.y = y;
  dst.width = TILE_SIZE;
  dst.height = TILE_SIZE;

  if (layer == 0) {
    _graphics->drawSprite("tiles", src, dst);
  } else {
    _graphics->drawSprite("elements", src, dst);
  }
}

}  // namespace bure

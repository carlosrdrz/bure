#pragma once

#include <memory>

#include "renderer.h"

namespace bure {

class game_map_renderer : public renderer {
 public:
  void render(int layer);
  void renderTile(int tile, int x, int y, int layer);
};

}  // namespace bure

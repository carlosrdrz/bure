#pragma once

#include <memory>

#include "renderer.h"

namespace bure {

class game_map_renderer : public renderer {
 public:
  void render(int layer);

 private:
  void renderTile(tile t, int x, int y, int width, int height);
};

}  // namespace bure

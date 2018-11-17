#pragma once

#include <memory>

#include "renderer.h"

namespace bure {

class entity_renderer : public renderer {
 public:
  void render(int layer);
};

}  // namespace bure

#pragma once

#include "components/position_component.h"
#include "components/animation_component.h"
#include "components/sprite_component.h"
#include "components/component.h"
#include "renderer.h"

using namespace bure::components;

namespace bure {

class entity_renderer : public renderer {
 public:
  void render(int layer);

 private:
  void renderSprite(position_component &p, sprite_component &s);
  void renderAnimation(position_component &p, animation_component &s);
};

}  // namespace bure

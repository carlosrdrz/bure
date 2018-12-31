#include "entity_renderer.h"

#include "engine.h"
#include "utils/logger.h"

namespace bure {

void entity_renderer::render(int layer) {
  for (auto& entity : bure::engine::get().getEntities()) {
    auto& e = entity.get();

    if (e.getLayer() != layer) continue;

    auto position = e.getComponentByType<position_component>();
    if (position == nullptr) continue;

    auto sprite = e.getComponentByType<sprite_component>();
    if (sprite != nullptr) {
      renderSprite(*position, *sprite);
      continue;
    }

    auto animation = e.getComponentByType<animation_component>();
    if (animation != nullptr) {
      auto animationSprite = animation->tickAndGetSprite();
      renderSprite(*position, animationSprite);
      continue;
    }

    // Render animation FSM
  }
}

void entity_renderer::renderSprite(position_component &p, sprite_component &s) {
  bure::rect dst = {p.getX(), p.getY(), s.getWidth(), s.getHeight()};
  _graphics->drawSprite(s.getSpriteID(), s.getSrcRect(), dst);
}

}  // namespace bure

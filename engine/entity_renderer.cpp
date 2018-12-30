#include "entity_renderer.h"

#include "engine.h"
#include "components/sprite_component.h"
#include "components/position_component.h"
#include "utils/logger.h"

namespace bure {

void entity_renderer::render(int layer) {
  for (auto& entity : bure::engine::get().getEntities()) {
    if (entity.get().getLayer() != layer) continue;

    auto sprite = entity.get().getComponentByType<bure::components::sprite_component>();
    auto position = entity.get().getComponentByType<bure::components::position_component>();
    if (sprite == nullptr || position == nullptr) continue;

    bure::rect dst = {position->getX(), position->getY(), sprite->getWidth(), sprite->getHeight()};
    _graphics->drawSprite(sprite->getSpriteID(), sprite->getSrcRect(), dst);
  }
}

}  // namespace bure

#include "drawing_system.h"

namespace bure {
namespace systems {

void drawing_system::init() {
  graphics = std::make_unique<graphics2>();
}

void drawing_system::update() {
  auto entities = engine::getInstance().getEntities();
  for (auto& entity : entities) {
    auto pc = entity.getComponentByType<position_component>();
    auto scbc = entity.getComponentByType<static_color_background_component>();
    auto sc = entity.getComponentByType<sprite_component>();
  }
}

}  // namespace systems
}  // namespace bure

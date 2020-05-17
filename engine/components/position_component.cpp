#include "position_component.h"

namespace bure {
namespace components {

void position_component::setPosition(bure::world_coords wc) { _position = wc; }

bure::world_coords position_component::getPosition() {
  return getAbsolutePosition();
}

bure::world_coords position_component::getAbsolutePosition() {
  auto pos = getParentPos();
  pos.x += _position.x;
  pos.y += _position.y;
  return pos;
}

bure::world_coords position_component::getRelativePosition() {
  return _position;
}

bure::world_coords position_component::getParentPos() {
  if (_entity.getParent() != nullptr) {
    auto p = *_entity.getParent()->getComponentByType<position_component>();
    return p.getPosition();
  } else {
    return bure::source_world_coords;
  }
}

}  // namespace components
}  // namespace bure

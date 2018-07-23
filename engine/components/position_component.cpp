#include "position_component.h"

namespace bure {
namespace components {

int position_component::getX() {
  if (_entity.getParent() == nullptr) {
    return x;
  } else {
    return getParentPositionComponent().getX() + x;
  }
}

int position_component::getY() {
  if (_entity.getParent() == nullptr) {
    return y;
  } else {
    return getParentPositionComponent().getY() + y;
  }
}

position_component& position_component::getParentPositionComponent() {
  return *_entity.getParent()->getComponentByType<position_component>();
}

}  // namespace components
}  // namespace bure

#include "position_component.h"

namespace bure {
namespace components {

void position_component::setCoords(int x, int y) {
  _x = x;
  _y = y;
}

int position_component::getX() {
  if (_entity.getParent() == nullptr) {
    return _x;
  } else {
    return getParentPos().getX() + _x;
  }
}

int position_component::getY() {
  if (_entity.getParent() == nullptr) {
    return _y;
  } else {
    return getParentPos().getY() + _y;
  }
}

position_component& position_component::getParentPos() {
  return *_entity.getParent()->getComponentByType<position_component>();
}

}  // namespace components
}  // namespace bure

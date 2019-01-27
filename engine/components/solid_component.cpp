#include "solid_component.h"

namespace bure {
namespace components {

void solid_component::setCoords(int x, int y) {
  _x = x;
  _y = y;
}

int solid_component::getX() {
  return _x;
}

int solid_component::getY() {
  return _y;
}

}  // namespace components
}  // namespace bure

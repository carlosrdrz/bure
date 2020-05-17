#include "map_position_component.h"

namespace bure {
namespace components {

void map_position_component::setPosition(bure::map_coords wc) {
  _position = wc;
}

bure::map_coords map_position_component::getPosition() { return _position; }

}  // namespace components
}  // namespace bure

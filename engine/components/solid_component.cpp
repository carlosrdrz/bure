#include "solid_component.h"

namespace bure {
namespace components {

void solid_component::setPosition(bure::map_coords wc) { _position = wc; }

bure::map_coords solid_component::getPosition() { return _position; }

}  // namespace components
}  // namespace bure

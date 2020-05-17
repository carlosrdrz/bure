#pragma once

#include "component.h"
#include "types.h"

namespace bure {
namespace components {

class position_component : public component {
  using component::component;

 public:
  void setPosition(bure::world_coords wc);
  bure::world_coords getPosition();
  bure::world_coords getAbsolutePosition();
  bure::world_coords getRelativePosition();

 private:
  bure::world_coords _position = bure::undefined_world_coords;
  bure::world_coords getParentPos();
};

}  // namespace components
}  // namespace bure

#pragma once

#include "component.h"
#include "types.h"

namespace bure {
namespace components {

class solid_component : public component {
  using component::component;

 public:
  void setPosition(bure::map_coords wc);
  bure::map_coords getPosition();

 private:
  bure::map_coords _position = bure::undefined_map_coords;
};

}  // namespace components
}  // namespace bure

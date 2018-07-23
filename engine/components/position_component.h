#pragma once

#include "component.h"

namespace bure {
namespace components {

class position_component : public component {
 public:
  explicit position_component(const bure::entities::entity& e) : component(e) {}

  int x, y;
  int getX();
  int getY();

 private:
  position_component& getParentPositionComponent();
};

}  // namespace components
}  // namespace bure

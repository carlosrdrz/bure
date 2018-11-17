#pragma once

#include "component.h"

namespace bure {
namespace components {

class position_component : public component {
 using component::component;

 public:
  void setCoords(int x, int y);
  int getX();
  int getY();

 private:
   position_component& getParentPos();
   int _x;
   int _y;
};

}  // namespace components
}  // namespace bure

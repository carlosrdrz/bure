#pragma once

#include "component.h"

namespace bure {
namespace components {

class solid_component : public component {
 using component::component;

 public:
  void setCoords(int x, int y);
  int getX();
  int getY();

 private:
   int _x;
   int _y;
};

}  // namespace components
}  // namespace bure

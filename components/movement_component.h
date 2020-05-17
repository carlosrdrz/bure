#pragma once

#include "components/component.h"
#include "types.h"

class movement_component : public bure::components::component {
  using component::component;

 public:
  int getVelocity();
  bure::direction getDirection();

  void setVelocity(int v);
  void setDirection(bure::direction d);

 private:
  bure::direction _direction = bure::direction::none;
  int _velocity = 2;
};

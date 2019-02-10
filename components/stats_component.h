#pragma once

#include "components/component.h"
#include "types.h"

class stats_component : public bure::components::component {
 using component::component;

 public:
  int getHP();
  int getMaxHP();

  void setHP(int hp);
  void setMaxHP(int hp);

 private:
   int _hp = 100;
   int _max_hp = 100;
};

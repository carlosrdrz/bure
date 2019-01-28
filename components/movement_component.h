#pragma once

#include "components/component.h"
#include "game_map.h"

enum class movement_state {
  walking_up,
  walking_down,
  walking_right,
  walking_left,
  standing_up,
  standing_down,
  standing_right,
  standing_left
};

class movement_component : public bure::components::component {
 using component::component;

 public:
  bure::map_coords getPosition();
  int getVelocity();
  movement_state getState();

  void setPosition(bure::map_coords mc);
  void setVelocity(int v);
  void setState(movement_state s);

 private:
   movement_state _state = movement_state::standing_down;
   int _velocity = 2;
   bure::map_coords _currentPosition = {-1, -1};
};

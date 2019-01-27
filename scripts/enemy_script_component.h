#pragma once

#include "components/script_component.h"
#include "../entities/enemy_entity.h"
#include "event_manager.h"

enum class direction {
  up, down, left, right, standing
};

class enemy_script_component : public bure::components::script_component {
 using script_component::script_component;

 public:
  void onInit() override;
  void onTick() override;

  void setPosition(bure::map_coords pos);

 private:
  int _character_px_movement = 1;
  bure::map_coords _currentPosition = {-1, -1};
  bure::map_coords _nextPosition = {-1, -1};
  direction _directionMoving;
  enemy_entity* _character;

  void moveDirection(direction d);
  void stopMoving();
  void keepMoving();
  void addMovement();
};

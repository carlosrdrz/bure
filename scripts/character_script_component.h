#pragma once

#include "../entities/character_entity.h"
#include "components/script_component.h"
#include "event_manager.h"

using namespace bure::components;

class character_script_component : public script_component {
  using script_component::script_component;

 public:
  void onInit() override;
  void onTick() override;

  void updateCamera();
  void setPosition(bure::map_coords pos);

 private:
  character_entity* _character;
  int _character_px_movement = 2;
  bure::map_coords _currentPosition = {-1, -1};
  bure::map_coords _nextPosition = {-1, -1};

  void processKeys();
  void keepMoving();

  void onKeyDown(SDL_Scancode key);
  void stopMoving();
};

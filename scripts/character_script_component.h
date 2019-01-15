#pragma once

#include "components/script_component.h"
#include "../entities/character_entity.h"
#include "event_manager.h"

class character_script_component : public bure::components::script_component {
 using script_component::script_component;

 public:
  void onInit() override;
  void onTick() override;

  void updateCamera();

 private:
  character_entity* _character;
  int _character_px_movement = 2;

  void onKeyDown(SDL_Scancode key);
  void onKeyUp();
};

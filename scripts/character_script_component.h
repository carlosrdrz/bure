#pragma once

#include "components/script_component.h"
#include "../entities/character_entity.h"
#include "event_manager.h"

class character_script_component : public bure::components::script_component {
 using script_component::script_component;

 public:
  void onInit() override;

 private:
  character_entity* _character;

  void onKeyDown(SDL_Event e);
  void onKeyUp(SDL_Event e);
};

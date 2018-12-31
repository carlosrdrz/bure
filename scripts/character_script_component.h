#pragma once

#include "components/script_component.h"
#include "event_manager.h"

class character_script_component : public bure::components::script_component {
 using script_component::script_component;

 public:
  void onInit() override;

 private:
  void onMove(SDL_Event e);
};

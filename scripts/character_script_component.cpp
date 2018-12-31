#include "character_script_component.h"
#include "engine.h"

void character_script_component::onInit() {
  bure::event_manager::get().addEventCallback(
      SDL_KEYDOWN, std::bind(&character_script_component::onMove, this,
                             std::placeholders::_1));
}

void character_script_component::onMove(SDL_Event e) {
  if (e.key.keysym.scancode == SDL_SCANCODE_A) {
    bure::engine::get().globalX--;
  } else if (e.key.keysym.scancode == SDL_SCANCODE_D) {
    bure::engine::get().globalX++;
  } else if (e.key.keysym.scancode == SDL_SCANCODE_W) {
    bure::engine::get().globalY--;
  } else if (e.key.keysym.scancode == SDL_SCANCODE_S) {
    bure::engine::get().globalY++;
  }
}

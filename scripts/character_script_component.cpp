#include "character_script_component.h"
#include "engine.h"

void character_script_component::onInit() {
  bure::event_manager::get().addEventCallback(
      SDL_KEYDOWN, std::bind(&character_script_component::onKeyDown, this,
                             std::placeholders::_1));
  bure::event_manager::get().addEventCallback(
      SDL_KEYUP, std::bind(&character_script_component::onKeyUp, this,
                             std::placeholders::_1));

  _character = dynamic_cast<character_entity*>(&_entity);
}

void character_script_component::onKeyDown(SDL_Event e) {
  switch (e.key.keysym.scancode) {
    case SDL_SCANCODE_A:
      bure::engine::get().globalX--;
      _character->setState(character_state::walking_left);
      break;
    case SDL_SCANCODE_D:
      bure::engine::get().globalX++;
      _character->setState(character_state::walking_right);
      break;
    case SDL_SCANCODE_W:
      bure::engine::get().globalY--;
      _character->setState(character_state::walking_up);
      break;
    case SDL_SCANCODE_S:
      bure::engine::get().globalY++;
      _character->setState(character_state::walking_down);
      break;
    default:
      break;
  }
}

void character_script_component::onKeyUp(SDL_Event e) {
  switch (e.key.keysym.scancode) {
    case SDL_SCANCODE_A:
      if (_character->getState() != character_state::walking_left) return;
      _character->setState(character_state::standing_left);
      break;
    case SDL_SCANCODE_D:
      if (_character->getState() != character_state::walking_right) return;
      _character->setState(character_state::standing_right);
      break;
    case SDL_SCANCODE_W:
      if (_character->getState() != character_state::walking_up) return;
      _character->setState(character_state::standing_up);
      break;
    case SDL_SCANCODE_S:
      if (_character->getState() != character_state::walking_down) return;
      _character->setState(character_state::standing_down);
      break;
    default:
      break;
  }
}

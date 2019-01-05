#include "components/position_component.h"
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
  auto camera = bure::engine::get().getCamera();
  character_state dest_state = _character->getState();
  auto position =
      _character->getComponentByType<bure::components::position_component>();
  auto map = bure::engine::get().getMap();
  auto layer = map->getLayer(2);

  // Get four points for the sprite

  int pointsX[] = {
    camera.x + position->getX(),
    camera.x + position->getX() + 32 * map->getScale(),
  };

  int pointsY[] = {
    camera.y + position->getY(),
    camera.y + position->getY() + 32 * map->getScale(),
  };

  switch (e.key.keysym.scancode) {
    case SDL_SCANCODE_A:
      dest_state = character_state::walking_left;
      pointsX[0] -= _character_px_movement;
      pointsX[1] -= _character_px_movement;
      break;
    case SDL_SCANCODE_D:
      dest_state = character_state::walking_right;
      pointsX[0] += _character_px_movement;
      pointsX[1] += _character_px_movement;
      break;
    case SDL_SCANCODE_W:
      dest_state = character_state::walking_up;
      pointsY[0] -= _character_px_movement;
      pointsY[1] -= _character_px_movement;
      break;
    case SDL_SCANCODE_S:
      dest_state = character_state::walking_down;
      pointsY[0] += _character_px_movement;
      pointsY[1] += _character_px_movement;
      break;
    default:
      break;
  }

  for (auto x : pointsX) {
    for (auto y : pointsY) {
      int destTileX = x / map->getTileWidth();
      int destTileY = y / map->getTileHeight();

      auto tile = layer.data[destTileX + (destTileY * map->getWidth())];
      if (tile != 0) return;
    }
  }

  _character->setState(dest_state);
  bure::engine::get().setCamera({
    pointsX[0] - position->getX(),
    pointsY[0] - position->getY(),
    camera.width, camera.height
  });
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

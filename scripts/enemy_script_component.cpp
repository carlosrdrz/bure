#include "components/position_component.h"
#include "enemy_script_component.h"
#include "engine.h"

void enemy_script_component::onInit() {
  _character = dynamic_cast<enemy_entity*>(&_entity);
  _directionMoving = direction::standing;
}

void enemy_script_component::onTick() {
  auto position =
    _character->getComponentByType<bure::components::position_component>();

  // if (keystates[SDL_SCANCODE_A]) {
  //   moveDirection(SDL_SCANCODE_A);
  // } else if (keystates[SDL_SCANCODE_D]) {
  //   moveDirection(SDL_SCANCODE_D);
  // } else if (keystates[SDL_SCANCODE_W]) {
  //   moveDirection(SDL_SCANCODE_W);
  // } else if (keystates[SDL_SCANCODE_S]) {
  //   moveDirection(SDL_SCANCODE_S);
  // } else {
  //   stopMoving();
  // }
}

void enemy_script_component::moveDirection(direction d) {
  // auto position =
  //     _character->getComponentByType<bure::components::position_component>();
  // auto camera = bure::engine::get().getCamera();
  // auto map = bure::engine::get().getMap();
  // auto layer = map->getLayer(2);
  //
  // int pointsX[] = {
  //   camera.x + position->getX(),
  //   camera.x + position->getX() + 31 * map->getScale(),
  // };
  //
  // int pointsY[] = {
  //   camera.y + position->getY(),
  //   camera.y + position->getY() + 31 * map->getScale(),
  // };
  //
  // switch (key) {
  //   case SDL_SCANCODE_A:
  //     _character->setState(character_state::walking_left);
  //     pointsX[0] -= _character_px_movement;
  //     pointsX[1] -= _character_px_movement;
  //     break;
  //   case SDL_SCANCODE_D:
  //     _character->setState(character_state::walking_right);
  //     pointsX[0] += _character_px_movement;
  //     pointsX[1] += _character_px_movement;
  //     break;
  //   case SDL_SCANCODE_W:
  //     _character->setState(character_state::walking_up);
  //     pointsY[0] -= _character_px_movement;
  //     pointsY[1] -= _character_px_movement;
  //     break;
  //   case SDL_SCANCODE_S:
  //     _character->setState(character_state::walking_down);
  //     pointsY[0] += _character_px_movement;
  //     pointsY[1] += _character_px_movement;
  //     break;
  //   default:
  //     break;
  // }
  //
  // for (auto x : pointsX) {
  //   for (auto y : pointsY) {
  //     int destTileX = x / map->getTileWidth();
  //     int destTileY = y / map->getTileHeight();
  //
  //     auto tile = layer.data[destTileX + (destTileY * map->getWidth())];
  //     if (tile != 0) return;
  //   }
  // }
  //
  // bure::engine::get().setCamera({
  //   pointsX[0] - position->getX(),
  //   pointsY[0] - position->getY(),
  //   camera.width, camera.height
  // });
}

void enemy_script_component::stopMoving() {
  // switch (_character->getState()) {
  //   case character_state::walking_left:
  //     _character->setState(character_state::standing_left);
  //     break;
  //   case character_state::walking_right:
  //     _character->setState(character_state::standing_right);
  //     break;
  //   case character_state::walking_up:
  //     _character->setState(character_state::standing_up);
  //     break;
  //   case character_state::walking_down:
  //     _character->setState(character_state::standing_down);
  //     break;
  //   default:
  //     break;
  // }
}

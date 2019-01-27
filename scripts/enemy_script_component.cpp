#include "components/position_component.h"
#include "components/solid_component.h"
#include "enemy_script_component.h"
#include "engine.h"

#include <random>

using namespace bure::components;

void enemy_script_component::onInit() {
  _character = dynamic_cast<enemy_entity*>(&_entity);
  _directionMoving = direction::standing;
}

void enemy_script_component::onTick() {
  keepMoving();

  if (_character->isStanding()) {
    addMovement();
  }
}

void enemy_script_component::moveDirection(direction d) {
  auto map = bure::engine::get().getMap();
  character_state nextState = _character->getState();
  bure::map_coords nextPosition = { -1, -1 };

  switch (d) {
    case direction::left:
      nextState = character_state::walking_left;
      nextPosition = {_currentPosition.x - 1, _currentPosition.y};
      break;
    case direction::right:
      nextState = character_state::walking_right;
      nextPosition = {_currentPosition.x + 1, _currentPosition.y};
      break;
    case direction::up:
      nextState = character_state::walking_up;
      nextPosition = {_currentPosition.x, _currentPosition.y - 1};
      break;
    case direction::down:
      nextState = character_state::walking_down;
      nextPosition = {_currentPosition.x, _currentPosition.y + 1};
      break;
    default:
      break;
  }

  if (map->canWalk(nextPosition)) {
    _character->setState(nextState);
    _nextPosition = nextPosition;
  }
}

void enemy_script_component::stopMoving() {
  switch (_character->getState()) {
    case character_state::walking_left:
      _character->setState(character_state::standing_left);
      break;
    case character_state::walking_right:
      _character->setState(character_state::standing_right);
      break;
    case character_state::walking_up:
      _character->setState(character_state::standing_up);
      break;
    case character_state::walking_down:
      _character->setState(character_state::standing_down);
      break;
    default:
      break;
  }
}

void enemy_script_component::keepMoving() {
  auto position = _character->getComponentByType<position_component>();
  auto map = bure::engine::get().getMap();
  auto layer = map->getLayer(2);
  auto nextPos = bure::screen_coords({position->getX(), position->getY()});
  bool isFinished = false;

  switch (_character->getState()) {
    case character_state::walking_left:
      nextPos.x -= _character_px_movement;
      isFinished = nextPos.x % map->getTileWidth() == 0;
      break;
    case character_state::walking_right:
      nextPos.x += _character_px_movement;
      isFinished = nextPos.x % map->getTileWidth() == 0;
      break;
    case character_state::walking_up:
      nextPos.y -= _character_px_movement;
      isFinished = nextPos.y % map->getTileHeight() == 0;
      break;
    case character_state::walking_down:
      nextPos.y += _character_px_movement;
      isFinished = nextPos.y % map->getTileHeight() == 0;
      break;
    default:
      break;
  }

  position->setCoords(nextPos.x, nextPos.y);

  if (isFinished) {
    _currentPosition = _nextPosition;
    auto solid = _character->getComponentByType<solid_component>();
    solid->setCoords(_currentPosition.x, _currentPosition.y);
    _nextPosition = {-1, -1};
    stopMoving();
  }
}

void enemy_script_component::addMovement() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 3);
  int randomNumber = dis(gen);

  switch (randomNumber) {
    case 0:
      moveDirection(direction::left);
      break;
    case 1:
      moveDirection(direction::right);
      break;
    case 2:
      moveDirection(direction::up);
      break;
    case 3:
      moveDirection(direction::down);
      break;
    default:
      break;
  }
}

void enemy_script_component::setPosition(bure::map_coords pos) {
  auto map = bure::engine::get().getMap();
  auto screen_pos = map->mapToScreen(pos);
  auto position = _character->getComponentByType<position_component>();
  auto solid = _character->getComponentByType<solid_component>();
  position->setCoords(screen_pos.x, screen_pos.y);
  _currentPosition = pos;
  solid->setCoords(_currentPosition.x, _currentPosition.y);
}

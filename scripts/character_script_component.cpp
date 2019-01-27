#include "character_script_component.h"
#include "components/position_component.h"
#include "components/solid_component.h"
#include "engine.h"

void character_script_component::onInit() {
  _character = dynamic_cast<character_entity*>(&_entity);
}

void character_script_component::onTick() {
  keepMoving();

  if (_character->isStanding()) {
    processKeys();
  }
}

void character_script_component::onKeyDown(SDL_Scancode key) {
  auto map = bure::engine::get().getMap();
  character_state nextState = _character->getState();
  bure::map_coords nextPosition = { -1, -1 };

  switch (key) {
    case SDL_SCANCODE_A:
      nextState = character_state::walking_left;
      nextPosition = {_currentPosition.x - 1, _currentPosition.y};
      break;
    case SDL_SCANCODE_D:
      nextState = character_state::walking_right;
      nextPosition = {_currentPosition.x + 1, _currentPosition.y};
      break;
    case SDL_SCANCODE_W:
      nextState = character_state::walking_up;
      nextPosition = {_currentPosition.x, _currentPosition.y - 1};
      break;
    case SDL_SCANCODE_S:
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

void character_script_component::onKeyUp() {
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

void character_script_component::updateCamera() {
  auto position = _character->getComponentByType<position_component>();
  auto camera = bure::engine::get().getCamera();
  auto map = bure::engine::get().getMap();

  bure::engine::get().setCamera(
      {position->getX() + map->getTileWidth() * map->getScale() / 2 -
           camera.width / 2,
       position->getY() + map->getTileHeight() * map->getScale() / 2 -
           camera.height / 2,
       camera.width, camera.height});
}

void character_script_component::setPosition(bure::map_coords pos) {
  auto map = bure::engine::get().getMap();
  auto screen_pos = map->mapToScreen(pos);
  auto position = _character->getComponentByType<position_component>();
  auto solid = _character->getComponentByType<solid_component>();
  position->setCoords(screen_pos.x, screen_pos.y);
  _currentPosition = pos;
  solid->setCoords(_currentPosition.x, _currentPosition.y);
}

void character_script_component::processKeys() {
  const Uint8* keystates = SDL_GetKeyboardState(NULL);

  if (keystates[SDL_SCANCODE_A]) {
    onKeyDown(SDL_SCANCODE_A);
  } else if (keystates[SDL_SCANCODE_D]) {
    onKeyDown(SDL_SCANCODE_D);
  } else if (keystates[SDL_SCANCODE_W]) {
    onKeyDown(SDL_SCANCODE_W);
  } else if (keystates[SDL_SCANCODE_S]) {
    onKeyDown(SDL_SCANCODE_S);
  } else {
    onKeyUp();
  }
}

void character_script_component::keepMoving() {
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
    onKeyUp();
  }

  updateCamera();
}

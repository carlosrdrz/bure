#include "player_entity.h"
#include "components/position_component.h"
#include "engine.h"

using namespace bure::components;

void player_entity::init() {
  character_entity::init();

  setPosition({66, 37});
  setVelocity(2);
}

void player_entity::update() {
  character_entity::update();

  updateCamera();

  if (isStanding()) {
    processKeys();
  } else {
    updateCamera();
  }
}

void player_entity::updateCamera() {
  auto position = getComponentByType<position_component>();
  auto camera = bure::engine::get().getCamera();
  auto map = bure::engine::get().getMap();
  bure::engine::get().setCamera(
      {position->getX() + map->getTileWidth() * map->getScale() / 2 -
           camera.width / 2,
       position->getY() + map->getTileHeight() * map->getScale() / 2 -
           camera.height / 2,
       camera.width, camera.height});
}

void player_entity::processKeys() {
  const Uint8* keystates = SDL_GetKeyboardState(NULL);

  if (keystates[SDL_SCANCODE_A]) {
    moveTo(direction::left);
  } else if (keystates[SDL_SCANCODE_D]) {
    moveTo(direction::right);
  } else if (keystates[SDL_SCANCODE_W]) {
    moveTo(direction::up);
  } else if (keystates[SDL_SCANCODE_S]) {
    moveTo(direction::down);
  }
}

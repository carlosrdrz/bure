#include "player_entity.h"
#include "../components/stats_component.h"
#include "../entities/enemy_entity.h"
#include "components/map_position_component.h"
#include "components/position_component.h"
#include "engine.h"
#include "fire_entity.h"
#include "../test_game.h"

using namespace bure::components;

void player_entity::init() {
  character_entity::init();

  setPosition({66, 37});
  setVelocity(2);
}

void player_entity::update() {
  character_entity::update();

  castSkills();
  updateCamera();

  if (isStanding()) {
    processKeys();
  } else {
    updateCamera();
  }
}

void player_entity::updateCamera() {
  auto pc = getComponentByType<position_component>();
  auto position = pc->getPosition();
  auto camera = bure::engine::get().getCamera();
  auto map = bure::engine::get().getMap();
  bure::engine::get().setCamera(
      {position.x + map->getTileWidth() * map->getScale() / 2 -
           camera.width / 2,
       position.y + map->getTileHeight() * map->getScale() / 2 -
           camera.height / 2,
       camera.width, camera.height});
}

void player_entity::processKeys() {
  const Uint8* keystates = SDL_GetKeyboardState(NULL);

  if (keystates[SDL_SCANCODE_A]) {
    moveTo(bure::direction::left);
  } else if (keystates[SDL_SCANCODE_D]) {
    moveTo(bure::direction::right);
  } else if (keystates[SDL_SCANCODE_W]) {
    moveTo(bure::direction::up);
  } else if (keystates[SDL_SCANCODE_S]) {
    moveTo(bure::direction::down);
  }
}

void player_entity::castSkills() {
  const Uint8* keystates = SDL_GetKeyboardState(NULL);

  if (_skillCooldownCounter == 0) {
    if (keystates[SDL_SCANCODE_Q]) {
      auto f = std::make_unique<fire_entity>();
      auto p = getComponentByType<map_position_component>();
      auto currentPos = p->getPosition();
      auto firePos = bure::map_coords({currentPos.x + 1, currentPos.y});
      f->setPosition(firePos);
      bure::engine::get().addEntity(std::move(f));
      _skillCooldownCounter = _skillCooldown;

      auto game = dynamic_cast<test_game*>(bure::engine::get().getGame());
      auto e = game->entityIn(firePos);
      if (e != nullptr) {
        auto stats = e->getComponentByType<stats_component>();
        if (stats != nullptr) {
          auto hp = stats->getHP();
          if (hp - 13 < 0) {
            bure::engine::get().removeEntity(e);
            auto e = std::make_unique<enemy_entity>();
            e->follow(this);
            bure::engine::get().addEntity(std::move(e));
          } else {
            stats->setHP(hp - 13);
          }
        }
      }
    }
  } else {
    _skillCooldownCounter--;
  }
}

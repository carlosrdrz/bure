#include "player_entity.h"
#include "../components/stats_component.h"
#include "../entities/enemy_entity.h"
#include "components/map_position_component.h"
#include "components/position_component.h"
#include "engine.h"
#include "fire_entity.h"
#include "../example_game.h"
#include "../utils/map_generator.h"

using namespace bure::components;

void player_entity::init() {
  character_entity::init();

  // TODO: WHY IN MAP_GENERATOR
  auto game = dynamic_cast<example_game*>(bure::engine::get().getGame());
  int x, y;

  do {
    x = map_generator::random(0, 60);
    y = map_generator::random(0, 60);
  } while (!game->canWalk({ x, y }));

  setPosition({x, y});
  setVelocity(8);
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
    if (keystates[SDL_SCANCODE_LCTRL]) {
      setDirection(bure::direction::left);
    } else {
      moveTo(bure::direction::left);
    }
  } else if (keystates[SDL_SCANCODE_D]) {
    if (keystates[SDL_SCANCODE_LCTRL]) {
      setDirection(bure::direction::right);
    } else {
      moveTo(bure::direction::right);
    }
  } else if (keystates[SDL_SCANCODE_W]) {
    if (keystates[SDL_SCANCODE_LCTRL]) {
      setDirection(bure::direction::up);
    } else {
      moveTo(bure::direction::up);
    }
  } else if (keystates[SDL_SCANCODE_S]) {
    if (keystates[SDL_SCANCODE_LCTRL]) {
      setDirection(bure::direction::down);
    } else {
      moveTo(bure::direction::down);
    }
  }
}

void player_entity::castSkills() {
  const Uint8* keystates = SDL_GetKeyboardState(NULL);

  if (_skillCooldownCounter == 0) {
    if (keystates[SDL_SCANCODE_Q]) {
      auto f = std::make_unique<fire_entity>();
      auto p = getComponentByType<map_position_component>();
      auto firePos = p->getPosition() + getDirection();
      f->setPosition(firePos);
      bure::engine::get().addEntity(std::move(f));
      _skillCooldownCounter = _skillCooldown;

      auto game = dynamic_cast<example_game*>(bure::engine::get().getGame());
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

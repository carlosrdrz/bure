#include "example_game.h"

#include "components/animation_component.h"
#include "components/map_position_component.h"
#include "engine.h"
#include "entities/background_entity.h"
#include "entities/enemy_entity.h"
#include "entities/player_entity.h"
#include "game_map_renderer.h"
#include "hud_renderer.h"
#include "systems/movement_system.h"
#include "utils/logger.h"
#include "utils/map_generator.h"
#include "utils/tiled_map_reader.h"

void example_game::init() {
  auto backgroundEntity = std::make_unique<background_entity>();
  bure::engine::get().addEntity(std::move(backgroundEntity));

  // add button to start game
  auto ui = bure::engine::get().getUIManager();
  ui->setScale(2);

  auto c = std::make_unique<bure::ui::container>(510, 300, 260, 120);
  c->setLayer(1);

  auto b = std::make_unique<bure::ui::button>("START GAME");
  b->set(40, 40, 180, 40);
  b->function =
      std::bind(&example_game::startGame, this, std::placeholders::_1);

  c->add(std::move(b));
  ui->addContainer(std::move(c));

  // create map generator
  _mapGenerator =
      std::make_unique<map_generator>("resources/maps/generator_config.xml");
}

void example_game::startGame(int unused) {
  // starts the actual game
  auto ui = bure::engine::get().getUIManager();
  ui->removeContainer(0);

  // add renderer for the components we will use
  auto gameMapRenderer = std::make_unique<bure::game_map_renderer>();
  auto hudRenderer = std::make_unique<hud_renderer>();
  bure::engine::get().addRenderer(std::move(gameMapRenderer));
  bure::engine::get().addRenderer(std::move(hudRenderer));

  // read map
  auto map = this->generateMap();
  map->setScale(2.0);

  bure::engine::get().clearEntities();
  bure::engine::get().setMap(std::move(map));

  // add movement system
  auto movementSystem = std::make_unique<movement_system>();
  bure::engine::get().addSystem(std::move(movementSystem));

  // add player character
  auto playerEntity = std::make_unique<player_entity>();
  player_entity* p = playerEntity.get();

  bure::event_manager::get().addEventCallback(SDL_KEYDOWN, [this,
                                                            p](SDL_Event e) {
    auto m = bure::engine::get().getMap();

    if (e.key.keysym.scancode == SDL_SCANCODE_4) {
      auto n = this->generateMap();
      n->setScale(m->getScale());
      bure::engine::get().setMap(std::move(n));
      p->updateCamera();
    }

    if (e.key.keysym.scancode == SDL_SCANCODE_5) {
      m->setScale(m->getScale() * 2);
      auto sc = p->getComponentByType<bure::components::animation_component>();
      sc->setScale(sc->getScale() * 2);
      auto mp =
          p->getComponentByType<bure::components::map_position_component>();
      auto pc = p->getComponentByType<bure::components::position_component>();
      pc->setPosition(m->mapToWorld(mp->getPosition()));
      p->updateCamera();
    }

    if (e.key.keysym.scancode == SDL_SCANCODE_6) {
      if (m->getScale() >= 0.5) {
        m->setScale(m->getScale() / 2);
        auto sc =
            p->getComponentByType<bure::components::animation_component>();
        sc->setScale(sc->getScale() / 2);
        auto mp =
            p->getComponentByType<bure::components::map_position_component>();
        auto pc = p->getComponentByType<bure::components::position_component>();
        pc->setPosition(m->mapToWorld(mp->getPosition()));
        p->updateCamera();
      }
    }

    if (e.key.keysym.scancode == SDL_SCANCODE_7) {
      _godMode = true;
    }

    if (e.key.keysym.scancode == SDL_SCANCODE_8) {
      _godMode = false;
    }
  });

  // add enemy and make it follow player around
  auto enemyEntity = std::make_unique<enemy_entity>();
  enemyEntity->follow(playerEntity.get());

  bure::engine::get().addEntity(std::move(playerEntity));
  bure::engine::get().addEntity(std::move(enemyEntity));
}

bool example_game::canWalk(bure::map_coords mc) {
  if (_godMode) return true;
  auto map = bure::engine::get().getMap();
  auto layer = map->getLayer(1);
  auto tile = layer.data[mc.x + (mc.y * map->getWidth())];
  return tile != 0 && !anyEntityIn(mc);
}

bool example_game::anyEntityIn(bure::map_coords mc) {
  auto entities = bure::engine::get().getEntities();

  for (auto& entity : entities) {
    auto& e = entity.get();
    auto solid =
        e.getComponentByType<bure::components::map_position_component>();
    if (solid == nullptr) continue;

    if (solid->getPosition() == mc) {
      return true;
    };
  }

  return false;
}

bure::entities::entity* example_game::entityIn(bure::map_coords mc) {
  auto entities = bure::engine::get().getEntities();

  for (auto& entity : entities) {
    auto& e = entity.get();
    auto mp = e.getComponentByType<bure::components::map_position_component>();
    if (mp == nullptr) continue;

    if (mp->getPosition() == mc) {
      return &e;
    };
  }

  return nullptr;
}

std::unique_ptr<bure::game_map> example_game::generateMap() {
  return _mapGenerator->generate(48, 48);
}

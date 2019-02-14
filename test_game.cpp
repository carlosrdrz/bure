#include "test_game.h"
#include "components/map_position_component.h"
#include "engine.h"
#include "entities/background_entity.h"
#include "entities/enemy_entity.h"
#include "entities/player_entity.h"
#include "systems/movement_system.h"
#include "utils/logger.h"
#include "utils/tiled_map_reader.h"

void test_game::init() {
  auto backgroundEntity = std::make_unique<background_entity>();
  bure::engine::get().addEntity(std::move(backgroundEntity));

  // add button to start game
  auto ui = bure::engine::get().getUIManager();
  ui->setScale(2);

  auto c = std::make_unique<bure::ui::container>(510, 300, 260, 120);
  c->setLayer(1);

  auto b = std::make_unique<bure::ui::button>("START GAME");
  b->set(40, 40, 180, 40);
  b->function = std::bind(&test_game::startGame, this, std::placeholders::_1);

  c->add(std::move(b));
  ui->addContainer(std::move(c));
}

void test_game::startGame(int unused) {
  // starts the actual game
  auto ui = bure::engine::get().getUIManager();
  ui->removeContainer(0);

  // read map
  auto map = tiled_map_reader::read("./resources/maps/campo.tmx");
  map->setScale(2);

  bure::engine::get().clearEntities();
  bure::engine::get().setMap(std::move(map));

  // add movement system
  auto movementSystem = std::make_unique<movement_system>();
  bure::engine::get().addSystem(std::move(movementSystem));

  // add player character
  auto playerEntity = std::make_unique<player_entity>();

  // add enemy and make it follow player around
  auto enemyEntity = std::make_unique<enemy_entity>();
  enemyEntity->follow(playerEntity.get());

  bure::engine::get().addEntity(std::move(playerEntity));
  bure::engine::get().addEntity(std::move(enemyEntity));
}

bool test_game::canWalk(bure::map_coords mc) {
  auto map = bure::engine::get().getMap();
  auto layer = map->getLayer(2);
  auto tile = layer.data[mc.x + (mc.y * map->getWidth())];
  return tile == 0 && !anyEntityIn(mc);
}

bool test_game::anyEntityIn(bure::map_coords mc) {
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

bure::entities::entity* test_game::entityIn(bure::map_coords mc) {
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

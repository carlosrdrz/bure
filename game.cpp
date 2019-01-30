#include "game.h"
#include "engine.h"
#include "utils/logger.h"
#include "entities/enemy_entity.h"
#include "entities/player_entity.h"
#include "entities/background_entity.h"
#include "systems/movement_system.h"

game::game()
    : finished(false) {}

void game::finishGame() {
  finished = true;
}

void game::startMenu() {
  auto backgroundEntity = std::make_unique<background_entity>();
  bure::engine::get().addEntity(std::move(backgroundEntity));

  // add button to start game
  auto ui = bure::engine::get().getUIManager();
  ui->setScale(2);

  auto c = std::make_unique<bure::ui::container>(510, 300, 260, 120);
  c->setLayer(1);

  auto b = std::make_unique<bure::ui::button>("START GAME");
  b->set(40, 40, 180, 40);
  b->function = std::bind(&game::startGame, this, std::placeholders::_1);

  c->add(std::move(b));
  ui->addContainer(std::move(c));
}

void game::startGame(int unused) {
  // starts the actual game
  auto ui = bure::engine::get().getUIManager();
  ui->removeContainer(0);

  bure::engine::get().clearEntities();
  bure::engine::get().setMap("campo.tmx");
  bure::engine::get().getMap()->setScale(2);

  // add movement system
  auto movementSystem = std::make_unique<movement_system>();
  bure::engine::get().addSystem(std::move(movementSystem));

  // add game character
  auto playerEntity = std::make_unique<player_entity>();
  bure::engine::get().addEntity(std::move(playerEntity));
  auto enemyEntity = std::make_unique<enemy_entity>();
  bure::engine::get().addEntity(std::move(enemyEntity));
}

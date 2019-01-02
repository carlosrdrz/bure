#include "game.h"
#include "engine.h"
#include "utils/logger.h"
#include "entities/character_entity.h"
#include "entities/background_entity.h"

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
  auto c = std::make_unique<bure::ui::container>(362, 309, 130, 60);
  c->setLayer(1);

  auto b = std::make_unique<bure::ui::button>("START GAME");
  b->set(20, 20, 90, 20);
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

  // add game character
  auto characterEntity = std::make_unique<character_entity>();
  bure::engine::get().addEntity(std::move(characterEntity));
}

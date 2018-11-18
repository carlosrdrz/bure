#include "game.h"
#include "engine.h"
#include "utils/logger.h"

game::game()
    : finished(false) {}

void game::finishGame() {
  finished = true;
}

void game::startMenu() {
  auto ui = bure::engine::get().getUIManager();
  auto c = std::make_unique<bure::ui::container>(362, 309, 130, 60);
  c->setLayer(1);

  auto b = std::make_unique<bure::ui::button>("START GAME");
  b->set(20, 20, 90, 20);
  b->function = [ui](int) -> void {
    bure::engine::get().clearEntities();
    bure::engine::get().setMap("campo.tmx");
    ui->removeContainer(0);
  };

  c->add(std::move(b));
  ui->addContainer(std::move(c));
}

#include <memory>
#include <set>
#include <vector>
#include <iostream>

#include "engine/engine.h"
#include "engine/event_manager.h"
#include "engine/events/event_close.h"
#include "engine/graphics.h"
#include "engine/utils/config.h"
#include "game.h"

bure::config bure::config::instance;

void buildStartMenu(bure::ui::ui_manager* ui, game* g) {
  auto c = std::make_unique<bure::ui::container>(362, 309, 130, 60);
  auto b = bure::ui::button("START GAME");
  b.set(20, 20, 90, 20);
  b.function = [g, ui](int) -> void {
    g->changeMap("campo.tmx");
    g->getPlayer().setPosition(30, 36);
    g->playing = true;
    ui->writing = false;
    ui->closeContainer(0);
  };

  c->add(b);
  ui->addContainer(std::move(c));
}

int main(int argc, char* argv[]) {
  std::string resourcesPath("./");
  if (argv[1] != nullptr) {
    resourcesPath = std::string(argv[1]);
  }

  // Init config file
  bure::config::instance.readFile(resourcesPath);

  // Init main objects
  auto graphicsInstance = std::make_unique<bure::graphics>(resourcesPath);
  auto gameInstance = std::make_unique<game>();
  auto gamePointer = gameInstance.get();
  auto eventManager = std::make_unique<bure::event_manager>();
  auto uiManager = std::make_unique<bure::ui::ui_manager>();

  // Init start menu
  buildStartMenu(uiManager.get(), gamePointer);

  // Register close callback
  eventManager->addEventCallback(bure::events::event_id::close,
                                 [gamePointer](const bure::events::event& e) {
                                   gamePointer->finishGame();
                                 });

  // Main game loop
  while (!gameInstance->finished) {
    if (gameInstance->playing) {
      graphicsInstance->clean();
      // graphicsInstance->draw(*gameInstance);
      gameInstance->nextFrame();
    } else {
      graphicsInstance->renderBackground();
    }

    graphicsInstance->draw(uiManager.get());
    graphicsInstance->flipBuffer();

    // manage events
    eventManager->pollEvent();
    SDL_Delay(10);
  }

  return EXIT_SUCCESS;
}

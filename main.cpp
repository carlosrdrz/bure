#include <memory>
#include <set>
#include <vector>

#include "engine/engine.h"
#include "engine/event_manager.h"
#include "engine/graphics.h"
#include "engine/utils/config.h"
#include "game.h"

bure::config bure::config::instance;

void buildStartMenu(game* g) {
  auto c = new bure::ui::container(362, 309, 130, 60);
  auto b = bure::ui::button("START GAME");
  b.set(20, 20, 90, 20);
  b.function = [g](int) -> void {
    g->changeMap("campo.tmx");
    g->getPlayer().setPosition(30, 36);
    g->playing = true;
    // engine::uiManager->writing = false;
    // engine::uiManager->closeContainer(0);
  };

  c->add(b);
  // engine::uiManager->addContainer(c);
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
  auto eventManager = std::make_unique<bure::event_manager>();

  // Init start menu
  buildStartMenu(gameInstance.get());

  // Main game loop
  while (!gameInstance->finished) {
    if (gameInstance->playing) {
      graphicsInstance->clean();
      // graphicsInstance->draw(*gameInstance);
      gameInstance->nextFrame();
    } else {
      graphicsInstance->renderBackground();
    }

    // graphicsInstance->draw(engine::uiManager);
    graphicsInstance->flipBuffer();

    // while (eventManager->poll_event()) eventManager->process_event();

    SDL_Delay(10);
  }

  return EXIT_SUCCESS;
}

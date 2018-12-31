#include <memory>

#include "game.h"
#include "engine.h"
#include "event_manager.h"

#include "utils/config.h"
#include "utils/logger.h"

bure::config bure::config::instance;

int main(int argc, char* argv[]) {
  std::string resourcesPath("./");
  if (argv[1] != nullptr) {
    resourcesPath = std::string(argv[1]);
  }

  // Init config file
  bure::config::instance.readFile(resourcesPath);
  bure::engine::get().init(resourcesPath);

  // Init main game object
  auto gameInstance = std::make_unique<game>();
  gameInstance->startMenu();

  // Register close callback
  bure::event_manager::get().addEventCallback(
    SDL_QUIT, [gamePointer = gameInstance.get()](SDL_Event e) {
      gamePointer->finishGame();
    });

  // Main game loop
  while (!gameInstance->finished) {
    bure::event_manager::get().pollEvent();
    bure::engine::get().update();
    SDL_Delay(10);
  }

  return EXIT_SUCCESS;
}

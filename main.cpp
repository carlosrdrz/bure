#include <memory>

#include "test_game.h"
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

  // Init main game object
  // Init engine at 1280x720 resolution
  auto game = std::make_unique<test_game>();
  bure::engine::get().init(std::move(game), resourcesPath, 1280, 720);

  // Main game loop
  while (!bure::engine::get().getGame()->isFinished()) {
    bure::event_manager::get().pollEvent();
    bure::engine::get().update();
    SDL_Delay(10);
  }

  return EXIT_SUCCESS;
}

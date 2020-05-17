#include <memory>

#include "engine.h"
#include "event_manager.h"
#include "example_game.h"
#include "utils/config.h"
#include "utils/logger.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SLEEP_BETWEEN_FRAMES_MS 10

bure::config bure::config::instance;

int main(int _argc, char* argv[]) {
  std::string resourcesPath("./");
  if (argv[1] != nullptr) {
    resourcesPath = std::string(argv[1]);
  }

  // Init config file
  bure::config::instance.readFile(resourcesPath);

  // Init main game object
  auto game = std::make_unique<example_game>();
  bure::engine::get().init(std::move(game), resourcesPath, SCREEN_WIDTH,
                           SCREEN_HEIGHT);

  // Main game loop
  while (!bure::engine::get().getGame()->isFinished()) {
    bure::event_manager::get().pollEvent();
    bure::engine::get().update();
    SDL_Delay(SLEEP_BETWEEN_FRAMES_MS);
  }

  return EXIT_SUCCESS;
}

#include <memory>

#include "engine/engine.h"
#include "engine/event_manager.h"
#include "engine/graphics.h"
#include "engine/systems/drawing_system.h"
#include "engine/utils/config.h"
#include "engine/utils/logger.h"
#include "game.h"
#include "ui/ui_renderer.h"

bure::config bure::config::instance;

void buildStartMenu(bure::ui::ui_manager* ui, game* g) {
  auto c = std::make_unique<bure::ui::container>(362, 309, 130, 60);
  auto b = std::make_unique<bure::ui::button>("START GAME");
  b->set(20, 20, 90, 20);
  b->function = [g, ui](int) -> void {
    g->changeMap("campo.tmx");
    g->getPlayer().setPosition(30, 36);
    g->playing = true;
    ui->closeContainer(0);
  };

  c->add(std::move(b));
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
  auto gameInstance = std::make_unique<game>();
  auto gamePointer = gameInstance.get();
  auto uiManager = std::make_shared<bure::ui::ui_manager>();

  auto graphicsInstance = std::make_unique<bure::graphics2>(resourcesPath);
  auto drawingSystem = std::make_unique<bure::systems::drawing_system>(
      std::move(graphicsInstance));
  auto uiRenderer = std::make_unique<bure::ui::ui_renderer>(uiManager);
  drawingSystem->addRenderer(std::move(uiRenderer));
  bure::engine::get().addSystem(std::move(drawingSystem));

  // Init start menu
  buildStartMenu(uiManager.get(), gamePointer);

  // Register close callback
  bure::event_manager::get().addEventCallback(
      SDL_QUIT, [gamePointer](SDL_Event e) { gamePointer->finishGame(); });

  // Main game loop
  while (!gameInstance->finished) {
    bure::event_manager::get().pollEvent();
    bure::engine::get().update();
    SDL_Delay(10);
  }

  return EXIT_SUCCESS;
}

#include <memory>

#include "game.h"
#include "engine.h"
#include "event_manager.h"
#include "graphics.h"
#include "game_map_renderer.h"
#include "entity_renderer.h"
#include "entities/entity.h"
#include "components/position_component.h"
#include "components/sprite_component.h"
#include "systems/drawing_system.h"
#include "ui/ui_renderer.h"
#include "utils/config.h"
#include "utils/logger.h"

bure::config bure::config::instance;

void buildStartMenu(bure::ui::ui_manager* ui, game* g) {
  auto c = std::make_unique<bure::ui::container>(362, 309, 130, 60);
  c->setLayer(1);

  auto b = std::make_unique<bure::ui::button>("START GAME");
  b->set(20, 20, 90, 20);
  b->function = [g, ui](int) -> void {
    bure::engine::get().clearEntities();
    bure::engine::get().setMap("campo.tmx");
    ui->removeContainer(0);
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

  auto graphicsInstance = std::make_unique<bure::graphics>(resourcesPath);
  auto drawingSystem = std::make_unique<bure::systems::drawing_system>(
      std::move(graphicsInstance));
  auto uiRenderer = std::make_unique<bure::ui::ui_renderer>(uiManager);
  auto entityRenderer = std::make_unique<bure::entity_renderer>();
  auto gameMapRenderer = std::make_unique<bure::game_map_renderer>();
  drawingSystem->addRenderer(std::move(uiRenderer));
  drawingSystem->addRenderer(std::move(entityRenderer));
  drawingSystem->addRenderer(std::move(gameMapRenderer));
  bure::engine::get().addSystem(std::move(drawingSystem));

  // Init start menu
  buildStartMenu(uiManager.get(), gamePointer);

  // Register close callback
  bure::event_manager::get().addEventCallback(
      SDL_QUIT, [gamePointer](SDL_Event e) { gamePointer->finishGame(); });

  // Just something silly so we can move around the map
  bure::event_manager::get().addEventCallback(
    SDL_KEYDOWN, [](SDL_Event e) {
      if (e.key.keysym.scancode == SDL_SCANCODE_A) {
        bure::engine::get().globalX--;
      } else if (e.key.keysym.scancode == SDL_SCANCODE_D) {
        bure::engine::get().globalX++;
      } else if (e.key.keysym.scancode == SDL_SCANCODE_W) {
        bure::engine::get().globalY--;
      } else if (e.key.keysym.scancode == SDL_SCANCODE_S) {
        bure::engine::get().globalY++;
      }
    });

  // add entity for background
  auto backgroundEntity = std::make_unique<bure::entities::entity>();
  auto spriteComponent =
      backgroundEntity->addComponent<bure::components::sprite_component>();
  auto positionComponent =
      backgroundEntity->addComponent<bure::components::position_component>();
  positionComponent->setCoords(0, 0);
  spriteComponent->setSpriteID("background");
  spriteComponent->setSrcRect({ 0, 0, 1024, 768 });
  spriteComponent->setSize(1024, 768);
  bure::engine::get().addEntity(std::move(backgroundEntity));

  // Main game loop
  while (!gameInstance->finished) {
    bure::event_manager::get().pollEvent();
    bure::engine::get().update();
    SDL_Delay(10);
  }

  return EXIT_SUCCESS;
}

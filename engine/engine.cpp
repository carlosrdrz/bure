#include "engine.h"
#include "entity_renderer.h"
#include "game_map_renderer.h"
#include "graphics.h"
#include "systems/drawing_system.h"
#include "ui/ui_renderer.h"

namespace bure {

engine* engine::_instance;

void engine::init(std::string resourcesPath, int width, int height) {
  _resourcesPath = resourcesPath;

  // initialize ui manager
  _uiManager = std::make_shared<bure::ui::ui_manager>();

  // initialize camera and graphics
  _camera = { 0, 0, width, height};
  auto graphicsInstance =
      std::make_unique<bure::graphics>(resourcesPath, width, height);

  // initialize drawing system
  auto drawingSystem = std::make_unique<bure::systems::drawing_system>(
      std::move(graphicsInstance));
  auto uiRenderer = std::make_unique<bure::ui::ui_renderer>(_uiManager);
  auto entityRenderer = std::make_unique<bure::entity_renderer>();
  auto gameMapRenderer = std::make_unique<bure::game_map_renderer>();
  drawingSystem->addRenderer(std::move(uiRenderer));
  drawingSystem->addRenderer(std::move(entityRenderer));
  drawingSystem->addRenderer(std::move(gameMapRenderer));
  this->addSystem(std::move(drawingSystem));
}

void engine::addSystem(std::unique_ptr<systems::system> s) {
  s->init();
  _systems.push_back(std::move(s));
}

void engine::addEntity(std::unique_ptr<entities::entity> e) {
  e->init();
  _entities.push_back(std::move(e));
}

void engine::clearEntities() {
  // TODO(carlosrdrz): do we need to call here deinit methods or smth?
  _entities.clear();
}

void engine::removeEntity(entities::entity* e) {
  for (auto i = _entities.begin(); i != _entities.end(); i++) {
    if ((*i).get() == e) {
      _entities.erase(i);
      return;
    }
  }
}

void engine::update() {
  for (auto&& s : _systems) {
    s->update();
  }
}

ui::ui_manager* engine::getUIManager() { return _uiManager.get(); }

std::vector<std::reference_wrapper<entities::entity>> engine::getEntities() {
  std::vector<std::reference_wrapper<entities::entity>> vector;
  for (auto&& entity : _entities) {
    vector.push_back(std::ref(*entity));
  }
  return vector;
}

void engine::setMap(std::string mapName) {
  _currentMap = std::make_unique<bure::game_map>("./", mapName);
}

game_map* engine::getMap() { return _currentMap.get(); }

void engine::setCamera(bure::camera c) {
  _camera = c;
}

camera engine::getCamera() {
  return _camera;
}

}  // namespace bure

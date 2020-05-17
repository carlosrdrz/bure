#include "engine.h"

#include "components/map_position_component.h"
#include "entity_renderer.h"
#include "graphics.h"
#include "systems/drawing_system.h"
#include "ui/ui_renderer.h"

namespace bure {

engine* engine::_instance;

void engine::init(std::unique_ptr<game> g, std::string resourcesPath, int width,
                  int height) {
  _game = std::move(g);
  _resourcesPath = resourcesPath;

  // initialize camera and graphics
  _camera = {0, 0, width, height};
  auto gr = std::make_unique<bure::graphics>(resourcesPath, width, height);

  // initialize drawing system
  auto ds = std::make_unique<bure::systems::drawing_system>(std::move(gr));
  this->addSystem(std::move(ds));

  // add ui renderer with manager
  _uiManager = std::make_shared<bure::ui::ui_manager>();
  auto uiRenderer = std::make_unique<bure::ui::ui_renderer>(_uiManager);
  addRenderer(std::move(uiRenderer));

  // add entity renderer
  auto entityRenderer = std::make_unique<bure::entity_renderer>();
  addRenderer(std::move(entityRenderer));

  // Register close callback
  bure::event_manager::get().addEventCallback(
      SDL_QUIT, [this](SDL_Event e) { _game->finish(); });

  // Start game
  _game->init();
}

void engine::addSystem(std::unique_ptr<systems::system> s) {
  s->init();
  _systems.push_back(std::move(s));
}

void engine::addEntity(std::unique_ptr<entities::entity> e) {
  e->init();
  _entities.push_back(std::move(e));
}

void engine::addRenderer(std::unique_ptr<renderer> r) {
  auto drawingSystem = getSystemByType<bure::systems::drawing_system>();
  if (drawingSystem != nullptr) {
    drawingSystem->addRenderer(std::move(r));
  }
}

void engine::clearEntities() {
  // TODO(carlosrdrz): do we need to call here deinit methods or smth?
  _entities.clear();
}

void engine::removeEntity(entities::entity* e) { _entitiesToRemove.emplace(e); }

void engine::update() {
  for (auto&& s : _systems) {
    s->update();
  }

  for (auto&& e : getEntities()) {
    if (_entitiesToRemove.find(&e.get()) == _entitiesToRemove.end()) {
      e.get().update();
    }
  }

  for (auto& e : _entitiesToRemove) {
    for (auto i = _entities.begin(); i != _entities.end(); i++) {
      if ((*i).get() == e) {
        _entities.erase(i);
        return;
      }
    }
  }

  _entitiesToRemove.clear();
}

ui::ui_manager* engine::getUIManager() { return _uiManager.get(); }

game* engine::getGame() { return _game.get(); }

std::vector<std::reference_wrapper<entities::entity>> engine::getEntities() {
  std::vector<std::reference_wrapper<entities::entity>> vector;
  for (auto&& entity : _entities) {
    if (_entitiesToRemove.find(entity.get()) == _entitiesToRemove.end()) {
      vector.push_back(std::ref(*entity));
    }
  }
  return vector;
}

void engine::setMap(std::unique_ptr<game_map> gm) {
  _currentMap = std::move(gm);
}

game_map* engine::getMap() { return _currentMap.get(); }

void engine::setCamera(bure::camera c) { _camera = c; }

camera engine::getCamera() { return _camera; }

}  // namespace bure

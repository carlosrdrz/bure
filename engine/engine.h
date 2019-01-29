#pragma once

#include <memory>
#include <vector>

#include "entities/entity.h"
#include "event_manager.h"
#include "game_map.h"
#include "systems/system.h"
#include "types.h"
#include "ui/ui_manager.h"

namespace bure {

class engine {
 public:
  void init(std::string resourcesPath, int width, int height);

  void addEntity(std::unique_ptr<entities::entity> e);
  void removeEntity(entities::entity* e);
  void clearEntities();

  void addSystem(std::unique_ptr<systems::system> s);

  std::vector<std::reference_wrapper<entities::entity>> getEntities();

  void setMap(std::string mapName);
  game_map* getMap();

  void update();

  ui::ui_manager* getUIManager();

  void setCamera(camera c);
  camera getCamera();

  static engine& get() {
    if (_instance == nullptr) {
      _instance = new engine();
    }

    return *_instance;
  }

 private:
  static engine* _instance;
  std::string _resourcesPath;
  camera _camera;

  std::vector<std::unique_ptr<bure::systems::system>> _systems;
  std::vector<std::unique_ptr<bure::entities::entity>> _entities;

  std::shared_ptr<bure::ui::ui_manager> _uiManager;
  std::unique_ptr<bure::game_map> _currentMap;
};

}  // namespace bure

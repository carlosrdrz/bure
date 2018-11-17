#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "ui/ui_manager.h"
#include "entities/entity.h"
#include "event_manager.h"
#include "systems/system.h"
#include "game_map.h"

namespace bure {

class engine {
 public:
  void addSystem(std::unique_ptr<systems::system> s);
  void addEntity(std::unique_ptr<entities::entity> e);

  void clearEntities();
  void removeEntity(entities::entity *e);

  std::vector<std::reference_wrapper<entities::entity>> getEntities();

  void setMap(std::string mapName);
  game_map* getMap();

  void update();

  // TODO(carlosrdrz): remove these
  // these are global position methods for debugging
  int globalX = 22;
  int globalY = 23;

  static engine& get() {
    if (_instance == nullptr) {
      _instance = new engine();
    }

    return *_instance;
  }
 private:
  static engine* _instance;
  std::unique_ptr<bure::game_map> _currentMap;
  std::unique_ptr<bure::ui::ui_manager> _uiManager;
  std::vector<std::unique_ptr<bure::systems::system>> _systems;
  std::vector<std::unique_ptr<bure::entities::entity>> _entities;
};

}  // namespace bure

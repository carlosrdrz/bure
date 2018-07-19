#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include "SDL.h"
#include "events/event.h"

namespace bure {

class event_manager {
 public:
  void addEventCallback(events::event_id event_id,
                        std::function<void(const events::event&)> fun) {
    std::cout << "adding callback for event id " << event_id << std::endl;
    _eventCallbacks[event_id].emplace_back(fun);
  }

  void pollEvent();
  void processEvent(const events::event& e);

  static event_manager& get() {
    if (_instance == nullptr) {
      _instance = new event_manager();
    }

    return *_instance;
  }

 private:
  static event_manager* _instance;

  std::map<events::event_id,
           std::list<std::function<void(const events::event&)>>>
      _eventCallbacks;

  SDL_Event lastEvent;
};

}  // namespace bure

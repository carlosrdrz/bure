#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <unordered_map>

#include "SDL.h"
#include "events/event.h"

namespace bure {

typedef uint_fast64_t callback_handler;

class event_manager {
 public:
  callback_handler addEventCallback(events::event_id event_id,
                        std::function<void(const events::event&)> fun);
  void removeEventCallback(events::event_id event_id, callback_handler ch);

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

  std::unordered_map<
      events::event_id,
      std::unordered_map<callback_handler,
                         std::function<void(const events::event&)>>>
      _eventCallbacks;

  SDL_Event lastEvent;
  callback_handler lastCallbackHandler;
};

}  // namespace bure

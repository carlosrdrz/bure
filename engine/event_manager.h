#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "SDL.h"

namespace bure {

typedef uint_fast64_t callback_handler;

class event_manager {
 public:
  event_manager();

  callback_handler addEventCallback(SDL_EventType event_id,
                        std::function<void(SDL_Event e)> fun);
  void removeEventCallback(SDL_EventType event_id, callback_handler ch);

  void pollEvent();

  static event_manager& get() {
    if (_instance == nullptr) {
      _instance = new event_manager();
    }

    return *_instance;
  }

 private:
  static event_manager* _instance;

  std::unordered_map<
      SDL_EventType,
      std::unordered_map<callback_handler,
                         std::function<void(SDL_Event e)>>>
      _eventCallbacks;

  SDL_Event lastEvent;
  callback_handler lastCallbackHandler;
  std::function<void(SDL_Event e)> funB;
};

}  // namespace bure

#include "event_manager.h"
#include "utils/logger.h"

namespace bure {

event_manager* event_manager::_instance;

event_manager::event_manager() {
  SDL_SetEventFilter([](void *userdata, SDL_Event *e) {
    switch (e->type) {
        // case SDL_QUIT:
        case SDL_MOUSEBUTTONDOWN:
        // case SDL_MOUSEBUTTONUP:
        // case SDL_KEYDOWN:
          return 1;
          break;
    }

    return 0;
  }, nullptr);
}

void event_manager::pollEvent() {
  SDL_Event lastEvent;

  while (SDL_PollEvent(&lastEvent)) {
    logger::debug("processing event id %d", lastEvent.type);
    funB(lastEvent);
    // for (auto it :
    //      _eventCallbacks[static_cast<SDL_EventType>(lastEvent.type)]) {
    //   it.second(lastEvent);
    // }
  }
}

callback_handler event_manager::addEventCallback(
    SDL_EventType event_type, std::function<void(SDL_Event e)> fun) {
  // _eventCallbacks[event_type][++lastCallbackHandler] = fun;
  funB = fun;
  return 1;
  // lastCallbackHandler;
}

void event_manager::removeEventCallback(SDL_EventType event_type,
                                        callback_handler ch) {
  _eventCallbacks[event_type].erase(ch);
}

}  // namespace bure

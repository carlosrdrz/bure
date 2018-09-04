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
    for (auto &st : _eventCallbacks) {
      if (st.event_type == lastEvent.type) st.callback(lastEvent);
      fun(lastEvent);
    }
  }
}

cb_handler event_manager::addEventCallback(
    SDL_EventType event_type, std::function<void(SDL_Event e)> fun) {
  auto handler = _eventCallbacks.size();
  _eventCallbacks.push_back(cb_struct{handler, fun, event_type});
  this->fun = fun;
  return handler;
}

void event_manager::removeEventCallback(SDL_EventType et, cb_handler ch) {
  for (auto it = _eventCallbacks.begin(); it != _eventCallbacks.end(); it++) {
    if ((*it).handler == ch) _eventCallbacks.erase(it);
  }
}

}  // namespace bure

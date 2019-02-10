#include "event_manager.h"
#include "utils/logger.h"
#include "utils/config.h"

namespace bure {

event_manager* event_manager::_instance;

event_manager::event_manager() {
  scale = bure::config::instance.getFloatValueOf("scale", 1.0);
  SDL_SetEventFilter([](void *userdata, SDL_Event *e) {
    switch (e->type) {
        case SDL_QUIT:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          return 1;
          break;
    }

    return 0;
  }, nullptr);
}

void event_manager::pollEvent() {
  SDL_Event lastEvent;

  while (SDL_PollEvent(&lastEvent)) {
    lastEvent.button.x = lastEvent.button.x / scale;
    lastEvent.button.y = lastEvent.button.y / scale;
    for (auto &st : _eventCallbacks) {
      if (st.event_type == lastEvent.type) st.callback(lastEvent);
    }
  }
}

cb_handler event_manager::addEventCallback(
    SDL_EventType event_type, std::function<void(SDL_Event e)> fun) {
  auto handler = _eventCallbacks.size();
  _eventCallbacks.push_back(cb_struct{handler, fun, event_type});
  return handler;
}

void event_manager::removeEventCallback(SDL_EventType et, cb_handler ch) {
  auto it = _eventCallbacks.begin();
  while (it != _eventCallbacks.end()) {
    if ((*it).handler == ch) {
      it = _eventCallbacks.erase(it);
    } else {
      ++it;
    }
  }
}

}  // namespace bure

#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "SDL.h"

namespace bure {

typedef uint_fast64_t cb_handler;
typedef std::function<void(SDL_Event e)> cb_callback;
struct cb_struct {
  cb_handler handler;
  cb_callback callback;
  SDL_EventType event_type;
};

class event_manager {
 public:
  event_manager();

  cb_handler addEventCallback(SDL_EventType event_id, cb_callback fun);
  void removeEventCallback(SDL_EventType event_id, cb_handler ch);

  void pollEvent();

  static event_manager& get() {
    if (_instance == nullptr) {
      _instance = new event_manager();
    }

    return *_instance;
  }

 private:
  static event_manager* _instance;
  std::vector<cb_struct> _eventCallbacks;
  float scale;
};

}  // namespace bure

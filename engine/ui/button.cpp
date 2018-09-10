#include "button.h"
#include "utils/logger.h"

using namespace std::placeholders;

namespace bure {
namespace ui {

button::button() : press{0}, parameter{0} {
}

button::button(std::string t) : title(t) { button(); }

void button::init() {
  bure::event_manager::get().addEventCallback(
    SDL_MOUSEBUTTONDOWN, std::bind(&button::onClickDown, this, _1));
  bure::event_manager::get().addEventCallback(
    SDL_MOUSEBUTTONUP, std::bind(&button::onClickUp, this, _1));
}

void button::onClickDown(SDL_Event e) {
  press = wasClicked(e.button.x, e.button.y) ? 1 : 0;
}

void button::onClickUp(SDL_Event e) {
  auto x = e.button.x;
  auto y = e.button.y;

  if (wasClicked(x, y)) {
    function(parameter);
    press = 1;
  } else {
    press = 0;
  }
}

}  // namespace ui
}  // namespace bure

#include "button.h"
#include "../engine/utils/logger.h"

using namespace std::placeholders;

namespace bure {
namespace ui {

button::button() : press{0}, parameter{0} {
  this->parent = nullptr;
  logger::debug("button constructor this %p parent %p", this, this->parent);

  bure::event_manager::get().addEventCallback(
    SDL_MOUSEBUTTONDOWN, std::bind(&button::onClickDown, this, _1));
  // bure::event_manager::get().addEventCallback(
  //     SDL_MOUSEBUTTONUP, std::bind(&button::onClickUp, this, _1));

  // logger::debug("button constructor2 this %p parent %p", this, this->parent);
}

button::button(std::string t) : title(t) { button(); }

const void button::onClickDown(SDL_Event e) {
  auto x = e.button.x;
  auto y = e.button.y;
  // logger::debug("checking if button was clicked on %d %d", x, y);

  logger::debug("callback this %p parent %p", this, this->parent);

  if (wasClicked(x, y)) {
    logger::debug("was clicked");
    press = 1;
  } else {
    logger::debug("wasnt clicked");
    press = 0;
  }
}

void button::onClickUp(SDL_Event e) {
  auto x = e.button.x;
  auto y = e.button.y;

  if (this->parent != nullptr) logger::debug("nopuedese");

  if (wasClicked(x, y)) {
    function(parameter);
    press = 1;
  } else {
    press = 0;
  }
}

}  // namespace ui
}  // namespace bure

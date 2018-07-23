#include <functional>

#include "ui_manager.h"

using namespace std::placeholders;

namespace bure {
namespace ui {

ui_manager::ui_manager() {
  writing = true;
  blocked = false;

  bure::event_manager::get().addEventCallback(
      SDL_MOUSEBUTTONDOWN, std::bind(&ui_manager::onClickDown, this, _1));
  bure::event_manager::get().addEventCallback(
      SDL_MOUSEBUTTONUP, std::bind(&ui_manager::onClickUp, this, _1));
}

ui_manager::~ui_manager() { _containers.clear(); }

void ui_manager::addContainer(std::unique_ptr<container> e) {
  e->index = static_cast<int>(_containers.size());
  _containers.push_back(std::move(e));
  if (static_cast<int>(_containers.size()) == 1) changeContainerFocus(0);
}

void ui_manager::closeContainer(int in) {
  std::vector<std::unique_ptr<container>>::iterator it;

  for (it = _containers.begin(); it != _containers.end(); ++it) {
    if ((*it)->index == in) {
      if ((*it)->index == getContainerFocused()->index) {
        changeContainerFocus(getContainerFocused()->index - 1);
      }

      _containers.erase(it);
      break;
    }
  }
  this->updateIndex();
}

void ui_manager::closeContainer(container* p) {
  if (NULL != p) {
    std::vector<std::unique_ptr<container>>::iterator it;

    for (it = _containers.begin(); it != _containers.end(); ++it) {
      if ((*it).get() == p) {
        if ((*it).get() == getContainerFocused()) {
          changeContainerFocus(getContainerFocused()->index - 1);
        }

        _containers.erase(it);
        break;
      }
    }
    this->updateIndex();
  }
}

void ui_manager::updateIndex() {
  int in = 0;
  for (auto&& c : _containers) {
    c->index = in++;
  }

  // if (static_cast<int>(containers.size()) == 1) {
  //   containerOnFocus = containers.begin();
  //   (*containerOnFocus)->focused = true;
  // }
}

container* ui_manager::getContainer(int cont) {
  if (cont > static_cast<int>(_containers.size()) - 1) return nullptr;

  for (auto&& c : _containers) {
    if (c->index == cont) {
      return c.get();
    }
  }

  return nullptr;
}

bool ui_manager::containerExists(container* p) {
  bool res = false;

  if (nullptr != p) {
    for (auto&& c : _containers) {
      if (c.get() == p) {
        res = true;
      }
    }
  }

  return res;
}

void ui_manager::changeContainerFocus(int num) {
  if (static_cast<int>(_containers.size()) > 1) {
    containerOnFocus->focused = false;
  }

  for (auto&& c : _containers) {
    if (c->index == num) {
      containerOnFocus = c.get();
      break;
    }
  }

  containerOnFocus->focused = true;
}

void ui_manager::changeInputBoxFocus(int num) {
  containerOnFocus->changeIBFocus(num);
}

void ui_manager::changeButtonFocus(int num) {
  containerOnFocus->changeButtonFocus(num);
}

container* ui_manager::getContainerFocused() {
  if (static_cast<int>(_containers.size()) == 0) return NULL;
  return containerOnFocus;
}

input_box* ui_manager::getInputBoxFocused() {
  if (static_cast<int>(_containers.size()) == 0) return NULL;
  return containerOnFocus->getIBFocused();
}

button* ui_manager::getButtonFocused() {
  return containerOnFocus->getButtonFocused();
}

bool ui_manager::clickOnContainer(int x, int y) {
  if (static_cast<int>(_containers.size()) > 0) {
    for (auto&& c : _containers) {
      if (x > c->x && x < (c->x + c->width) && y > c->y &&
          y < (c->y + c->height)) {
        return true;
      }
    }
  }
  return false;
}

bool ui_manager::clickOnInputBox(int x, int y) {
  int z;
  if (static_cast<int>(_containers.size()) > 0) {
    for (z = 0; getContainerFocused()->get_input_box(z) != NULL; ++z) {
      if (x > (getContainerFocused()->x +
               getContainerFocused()->get_input_box(z)->x) &&
          x < (getContainerFocused()->x +
               getContainerFocused()->get_input_box(z)->x +
               getContainerFocused()->get_input_box(z)->width) &&
          y > (getContainerFocused()->y +
               getContainerFocused()->get_input_box(z)->y) &&
          y < (getContainerFocused()->y +
               getContainerFocused()->get_input_box(z)->y +
               getContainerFocused()->get_input_box(z)->height)) {
        return true;
      }
    }
  }
  return false;
}

bool ui_manager::clickOnButton(int x, int y) {
  int z;
  if (static_cast<int>(_containers.size()) > 0) {
    for (z = 0; getContainerFocused()->get_button(z) != NULL; ++z) {
      if (x > (getContainerFocused()->x +
               getContainerFocused()->get_button(z)->x) &&
          x < (getContainerFocused()->x +
               getContainerFocused()->get_button(z)->x +
               getContainerFocused()->get_button(z)->width) &&
          y > (getContainerFocused()->y +
               getContainerFocused()->get_button(z)->y) &&
          y < (getContainerFocused()->y +
               getContainerFocused()->get_button(z)->y +
               getContainerFocused()->get_button(z)->height)) {
        return true;
      }
    }
  }
  return false;
}

bool ui_manager::clickOnSelector(int x, int y) {
  int z;
  if (_containers.size() > 0) {
    for (z = 0; getContainerFocused()->get_selector(z) != NULL; ++z) {
      if ((x > (getContainerFocused()->x +
                getContainerFocused()->get_selector(z)->x) &&
           x < (getContainerFocused()->x +
                getContainerFocused()->get_selector(z)->x + 9) &&
           y > (getContainerFocused()->y +
                getContainerFocused()->get_selector(z)->y) &&
           y < (getContainerFocused()->y +
                getContainerFocused()->get_selector(z)->y + 14)) ||
          (x > (getContainerFocused()->x +
                getContainerFocused()->get_selector(z)->x +
                getContainerFocused()->get_selector(z)->width + 17) &&
           x < (getContainerFocused()->x +
                getContainerFocused()->get_selector(z)->x +
                getContainerFocused()->get_selector(z)->width + 26) &&
           y > (getContainerFocused()->y +
                getContainerFocused()->get_selector(z)->y) &&
           y < (getContainerFocused()->y +
                getContainerFocused()->get_selector(z)->y + 14))) {
        return true;
      }
      /*

      */
    }
  }
  return false;
}

container* ui_manager::getContainerClicked(int x, int y) {
  container* result = nullptr;

  for (auto&& c : _containers) {
    if (x > c->x && x < (c->x + c->width) && y > c->y &&
        y < (c->y + c->height)) {
      result = c.get();
    }
  }

  return result;
}

input_box* ui_manager::getInputBoxClicked(int x, int y) {
  int z;
  input_box* toreturn = NULL;
  for (z = 0; getContainerFocused()->get_input_box(z) != NULL; ++z) {
    if (x > (getContainerFocused()->x +
             getContainerFocused()->get_input_box(z)->x) &&
        x < (getContainerFocused()->x +
             getContainerFocused()->get_input_box(z)->x +
             getContainerFocused()->get_input_box(z)->width) &&
        y > (getContainerFocused()->y +
             getContainerFocused()->get_input_box(z)->y) &&
        y < (getContainerFocused()->y +
             getContainerFocused()->get_input_box(z)->y +
             getContainerFocused()->get_input_box(z)->height)) {
      toreturn = getContainerFocused()->get_input_box(z);
    }
  }
  return toreturn;
}

button* ui_manager::getButtonClicked(int x, int y) {
  int z;
  button* toreturn = NULL;
  for (z = 0; getContainerFocused()->get_button(z) != NULL; ++z) {
    if (x > (getContainerFocused()->x +
             getContainerFocused()->get_button(z)->x) &&
        x < (getContainerFocused()->x +
             getContainerFocused()->get_button(z)->x +
             getContainerFocused()->get_button(z)->width) &&
        y > (getContainerFocused()->y +
             getContainerFocused()->get_button(z)->y) &&
        y < (getContainerFocused()->y +
             getContainerFocused()->get_button(z)->y +
             getContainerFocused()->get_button(z)->height)) {
      toreturn = getContainerFocused()->get_button(z);
    }
  }
  return toreturn;
}

selector* ui_manager::getSelectorClicked(int x, int y) {
  int z;
  selector* toreturn = NULL;
  for (z = 0; getContainerFocused()->get_selector(z) != NULL; ++z) {
    if ((x > (getContainerFocused()->x +
              getContainerFocused()->get_selector(z)->x) &&
         x < (getContainerFocused()->x +
              getContainerFocused()->get_selector(z)->x + 9) &&
         y > (getContainerFocused()->y +
              getContainerFocused()->get_selector(z)->y) &&
         y < (getContainerFocused()->y +
              getContainerFocused()->get_selector(z)->y + 14)) ||
        (x > (getContainerFocused()->x +
              getContainerFocused()->get_selector(z)->x +
              getContainerFocused()->get_selector(z)->width + 17) &&
         x < (getContainerFocused()->x +
              getContainerFocused()->get_selector(z)->x +
              getContainerFocused()->get_selector(z)->width + 26) &&
         y > (getContainerFocused()->y +
              getContainerFocused()->get_selector(z)->y) &&
         y < (getContainerFocused()->y +
              getContainerFocused()->get_selector(z)->y + 14))) {
      toreturn = getContainerFocused()->get_selector(z);
    }
  }
  return toreturn;
}

//////////////////////////
// ACCIONES DE LOS BOTONES
//////////////////////////
void ui_manager::execButton(button* button) {
  logger::debug("clicked in button!");
  button->function(button->parameter);
}

void ui_manager::onClickDown(SDL_Event e) {
  int x = e.button.x / 2;
  int y = e.button.y / 2;

  if (!blocked && writing) {
    if (e.button.button == SDL_BUTTON_LEFT) {
      if (clickOnContainer(x, y)) {
        auto container = getContainerClicked(x, y);
        if (!container->focused) changeContainerFocus(container->index);
      }

      if (clickOnInputBox(x, y)) {
        changeInputBoxFocus(getInputBoxClicked(x, y)->index);
      } else if (clickOnButton(x, y)) {
        changeButtonFocus(getButtonClicked(x, y)->index);
        getButtonFocused()->press = 1;
        getContainerClicked(x, y)->buttonPressed = true;
      } else if (clickOnSelector(x, y)) {
        if (x > (getContainerFocused()->x + getSelectorClicked(x, y)->x) &&
            x < (getContainerFocused()->x + getSelectorClicked(x, y)->x + 9) &&
            y > (getContainerFocused()->y + getSelectorClicked(x, y)->y) &&
            y < (getContainerFocused()->y + getSelectorClicked(x, y)->y + 14)) {
          getSelectorClicked(x, y)->previous();
        } else {
          getSelectorClicked(x, y)->next();
        }
      }
    }
  }
}

void ui_manager::onClickUp(SDL_Event e) {
  int x = e.button.x / 2;
  int y = e.button.y / 2;

  if (!blocked && writing && getContainerFocused() != nullptr) {
    if (getContainerFocused()->buttonPressed) {
      getButtonFocused()->press = 0;
      getContainerFocused()->buttonPressed = false;
      if (clickOnButton(x, y)) {
        if (getButtonClicked(x, y) == getButtonFocused())
          execButton(getButtonClicked(x, y));
      }
    }
  }
}

}  // namespace ui
}  // namespace bure

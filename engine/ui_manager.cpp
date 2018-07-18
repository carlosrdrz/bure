#include <sstream>

#include "SDL.h"
#include "ui_manager.h"
#include "graphics.h"
#include "ui/container.h"
#include "utils/config.h"
#include "utils/error.h"

using namespace std::placeholders;
using namespace bure::ui;

namespace bure {
ui_manager::ui_manager() {
  writing = true;
  blocked = false;
}

ui_manager::~ui_manager() { containers.clear(); }

void ui_manager::addContainer(container *e) {
  e->index = static_cast<int>(containers.size());
  containers.push_back(e);
  if (static_cast<int>(containers.size()) == 1) changeContainerFocus(0);
}

void ui_manager::closeContainer(int in) {
  std::list<container *>::iterator it;

  for (it = containers.begin(); it != containers.end(); ++it) {
    if ((*it)->index == in) {
      if ((*it)->index == getContainerFocused()->index) {
        changeContainerFocus(getContainerFocused()->index - 1);
      }

      containers.erase(it);
      break;
    }
  }
  this->updateIndex();
}

void ui_manager::closeContainer(container *p) {
  if (NULL != p) {
    std::list<container *>::iterator it;

    for (it = containers.begin(); it != containers.end(); ++it) {
      if ((*it) == p) {
        if ((*it) == getContainerFocused()) {
          changeContainerFocus(getContainerFocused()->index - 1);
        }

        containers.erase(it);
        break;
      }
    }
    this->updateIndex();
  }
}

void ui_manager::updateIndex() {
  std::list<container *>::iterator it;
  int in = 0;
  for (it = containers.begin(); it != containers.end(); ++it) {
    (*it)->index = in;
    in++;
  }

  if (static_cast<int>(containers.size()) == 1) {
    containerOnFocus = containers.begin();
    (*containerOnFocus)->focused = true;
  }
}

container *ui_manager::getContainer(int cont) {
  std::list<container *>::iterator it;

  if (cont > static_cast<int>(containers.size()) - 1) return NULL;

  for (it = containers.begin(); it != containers.end(); ++it) {
    if ((*it)->index == cont) {
      return (*it);
    }
  }

  return NULL;
}

bool ui_manager::containerExists(container *p) {
  bool res = false;
  std::list<container *>::iterator it;

  if (NULL != p) {
    for (it = containers.begin(); it != containers.end(); ++it) {
      if ((*it) == p) {
        res = true;
      }
    }
  }

  return res;
}

void ui_manager::changeContainerFocus(int num) {
  if (static_cast<int>(containers.size()) > 1) {
    (*containerOnFocus)->focused = false;
  }

  std::list<container *>::iterator it;
  for (it = containers.begin(); it != containers.end(); ++it) {
    if ((*it)->index == num) {
      containerOnFocus = it;
      break;
    }
  }

  (*containerOnFocus)->focused = true;
}

void ui_manager::changeInputBoxFocus(int num) {
  (*containerOnFocus)->changeIBFocus(num);
}

void ui_manager::changeButtonFocus(int num) {
  (*containerOnFocus)->changeButtonFocus(num);
}

container *ui_manager::getContainerFocused() {
  if (static_cast<int>(containers.size()) == 0) return NULL;
  return (*containerOnFocus);
}

input_box *ui_manager::getInputBoxFocused() {
  if (static_cast<int>(containers.size()) == 0) return NULL;
  return (*containerOnFocus)->getIBFocused();
}

button *ui_manager::getButtonFocused() {
  return (*containerOnFocus)->getButtonFocused();
}

bool ui_manager::clickOnContainer(int x, int y) {
  if (static_cast<int>(containers.size()) > 0) {
    std::list<container *>::iterator itib;
    for (itib = containers.begin(); itib != containers.end(); ++itib) {
      if (x > (*itib)->x && x < ((*itib)->x + (*itib)->width) &&
          y > (*itib)->y && y < ((*itib)->y + (*itib)->height)) {
        return true;
      }
    }
  }
  return false;
}

bool ui_manager::clickOnInputBox(int x, int y) {
  int z;
  if (static_cast<int>(containers.size()) > 0) {
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
  if (static_cast<int>(containers.size()) > 0) {
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
  if (containers.size() > 0) {
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

container *ui_manager::getContainerClicked(int x, int y) {
  std::list<container *>::iterator itib, toreturn;
  toreturn = containers.end();
  for (itib = containers.begin(); itib != containers.end(); ++itib) {
    if (x > (*itib)->x && x < ((*itib)->x + (*itib)->width) && y > (*itib)->y &&
        y < ((*itib)->y + (*itib)->height)) {
      toreturn = itib;
    }
  }

  if (toreturn == containers.end()) return NULL;

  return (*toreturn);
}

input_box *ui_manager::getInputBoxClicked(int x, int y) {
  int z;
  input_box *toreturn = NULL;
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

button *ui_manager::getButtonClicked(int x, int y) {
  int z;
  button *toreturn = NULL;
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

selector *ui_manager::getSelectorClicked(int x, int y) {
  int z;
  selector *toreturn = NULL;
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
void ui_manager::execButton(button *button) {
  button->function(button->parameter);
}
}  // namespace bure

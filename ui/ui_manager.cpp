#include <functional>
#include <set>

#include "ui_manager.h"

namespace bure {
namespace ui {

ui_manager::ui_manager() {
  writing = true;
  blocked = false;
}

ui_manager::~ui_manager() { _containers.clear(); }

void ui_manager::addContainer(std::unique_ptr<container> c) {
  c->index = static_cast<int>(_containers.size());
  if (static_cast<int>(_containers.size()) == 0) c->focused = true;
  _containers.push_back(std::move(c));
}

void ui_manager::closeContainer(int in) {
  std::vector<std::unique_ptr<container>>::iterator it;

  for (it = _containers.begin(); it != _containers.end(); ++it) {
    if ((*it)->index == in) {
      // if ((*it)->index == getContainerFocused()->index) {
      //   changeContainerFocus(getContainerFocused()->index - 1);
      // }

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
        // if ((*it).get() == getContainerFocused()) {
        //   changeContainerFocus(getContainerFocused()->index - 1);
        // }

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
//
// void ui_manager::changeContainerFocus(int num) {
//   if (static_cast<int>(_containers.size()) > 1) {
//     containerOnFocus->focused = false;
//   }
//
//   for (auto&& c : _containers) {
//     if (c->index == num) {
//       containerOnFocus = c.get();
//       break;
//     }
//   }
//
//   containerOnFocus->focused = true;
// }

// void ui_manager::changeInputBoxFocus(int num) {
//   containerOnFocus->changeIBFocus(num);
// }
//
// void ui_manager::changeButtonFocus(int num) {
//   containerOnFocus->changeButtonFocus(num);
// }

// container* ui_manager::getContainerFocused() {
//   if (static_cast<int>(_containers.size()) == 0) return NULL;
//   return containerOnFocus;
// }
//
// input_box* ui_manager::getInputBoxFocused() {
//   if (static_cast<int>(_containers.size()) == 0) return NULL;
//   return containerOnFocus->getIBFocused();
// }
//
// button* ui_manager::getButtonFocused() {
//   return containerOnFocus->getButtonFocused();
// }

// bool ui_manager::clickOnContainer(int x, int y) {
//   if (static_cast<int>(_containers.size()) > 0) {
//     for (auto&& c : _containers) {
//       if (x > c->x && x < (c->x + c->width) && y > c->y &&
//           y < (c->y + c->height)) {
//         return true;
//       }
//     }
//   }
//   return false;
// }

// bool ui_manager::clickOnInputBox(int x, int y) {
//   int z;
//   if (static_cast<int>(_containers.size()) > 0) {
//     for (z = 0; getContainerFocused()->get_input_box(z) != NULL; ++z) {
//       if (x > (getContainerFocused()->x +
//                getContainerFocused()->get_input_box(z)->x) &&
//           x < (getContainerFocused()->x +
//                getContainerFocused()->get_input_box(z)->x +
//                getContainerFocused()->get_input_box(z)->width) &&
//           y > (getContainerFocused()->y +
//                getContainerFocused()->get_input_box(z)->y) &&
//           y < (getContainerFocused()->y +
//                getContainerFocused()->get_input_box(z)->y +
//                getContainerFocused()->get_input_box(z)->height)) {
//         return true;
//       }
//     }
//   }
//   return false;
// }

// bool ui_manager::clickOnButton(int x, int y) {
//   int z;
//   if (static_cast<int>(_containers.size()) > 0) {
//     for (z = 0; getContainerFocused()->get_button(z) != NULL; ++z) {
//       if (x > (getContainerFocused()->x +
//                getContainerFocused()->get_button(z)->x) &&
//           x < (getContainerFocused()->x +
//                getContainerFocused()->get_button(z)->x +
//                getContainerFocused()->get_button(z)->width) &&
//           y > (getContainerFocused()->y +
//                getContainerFocused()->get_button(z)->y) &&
//           y < (getContainerFocused()->y +
//                getContainerFocused()->get_button(z)->y +
//                getContainerFocused()->get_button(z)->height)) {
//         return true;
//       }
//     }
//   }
//   return false;
// }

// bool ui_manager::clickOnSelector(int x, int y) {
//   int z;
//   if (_containers.size() > 0) {
//     for (z = 0; getContainerFocused()->get_selector(z) != NULL; ++z) {
//       if ((x > (getContainerFocused()->x +
//                 getContainerFocused()->get_selector(z)->x) &&
//            x < (getContainerFocused()->x +
//                 getContainerFocused()->get_selector(z)->x + 9) &&
//            y > (getContainerFocused()->y +
//                 getContainerFocused()->get_selector(z)->y) &&
//            y < (getContainerFocused()->y +
//                 getContainerFocused()->get_selector(z)->y + 14)) ||
//           (x > (getContainerFocused()->x +
//                 getContainerFocused()->get_selector(z)->x +
//                 getContainerFocused()->get_selector(z)->width + 17) &&
//            x < (getContainerFocused()->x +
//                 getContainerFocused()->get_selector(z)->x +
//                 getContainerFocused()->get_selector(z)->width + 26) &&
//            y > (getContainerFocused()->y +
//                 getContainerFocused()->get_selector(z)->y) &&
//            y < (getContainerFocused()->y +
//                 getContainerFocused()->get_selector(z)->y + 14))) {
//         return true;
//       }
//       /*
//
//       */
//     }
//   }
//   return false;
// }

// container* ui_manager::getContainerClicked(int x, int y) {
//   container* result = nullptr;
//
//   for (auto&& c : _containers) {
//     if (x > c->x && x < (c->x + c->width) && y > c->y &&
//         y < (c->y + c->height)) {
//       result = c.get();
//     }
//   }
//
//   return result;
// }

// input_box* ui_manager::getInputBoxClicked(int x, int y) {
//   int z;
//   input_box* toreturn = NULL;
//   for (z = 0; getContainerFocused()->get_input_box(z) != NULL; ++z) {
//     if (x > (getContainerFocused()->x +
//              getContainerFocused()->get_input_box(z)->x) &&
//         x < (getContainerFocused()->x +
//              getContainerFocused()->get_input_box(z)->x +
//              getContainerFocused()->get_input_box(z)->width) &&
//         y > (getContainerFocused()->y +
//              getContainerFocused()->get_input_box(z)->y) &&
//         y < (getContainerFocused()->y +
//              getContainerFocused()->get_input_box(z)->y +
//              getContainerFocused()->get_input_box(z)->height)) {
//       toreturn = getContainerFocused()->get_input_box(z);
//     }
//   }
//   return toreturn;
// }
//
// button* ui_manager::getButtonClicked(int x, int y) {
//   int z;
//   button* toreturn = NULL;
//   for (z = 0; getContainerFocused()->get_button(z) != NULL; ++z) {
//     if (x > (getContainerFocused()->x +
//              getContainerFocused()->get_button(z)->x) &&
//         x < (getContainerFocused()->x +
//              getContainerFocused()->get_button(z)->x +
//              getContainerFocused()->get_button(z)->width) &&
//         y > (getContainerFocused()->y +
//              getContainerFocused()->get_button(z)->y) &&
//         y < (getContainerFocused()->y +
//              getContainerFocused()->get_button(z)->y +
//              getContainerFocused()->get_button(z)->height)) {
//       toreturn = getContainerFocused()->get_button(z);
//     }
//   }
//   return toreturn;
// }
//
// selector* ui_manager::getSelectorClicked(int x, int y) {
//   int z;
//   selector* toreturn = NULL;
//   for (z = 0; getContainerFocused()->get_selector(z) != NULL; ++z) {
//     if ((x > (getContainerFocused()->x +
//               getContainerFocused()->get_selector(z)->x) &&
//          x < (getContainerFocused()->x +
//               getContainerFocused()->get_selector(z)->x + 9) &&
//          y > (getContainerFocused()->y +
//               getContainerFocused()->get_selector(z)->y) &&
//          y < (getContainerFocused()->y +
//               getContainerFocused()->get_selector(z)->y + 14)) ||
//         (x > (getContainerFocused()->x +
//               getContainerFocused()->get_selector(z)->x +
//               getContainerFocused()->get_selector(z)->width + 17) &&
//          x < (getContainerFocused()->x +
//               getContainerFocused()->get_selector(z)->x +
//               getContainerFocused()->get_selector(z)->width + 26) &&
//          y > (getContainerFocused()->y +
//               getContainerFocused()->get_selector(z)->y) &&
//          y < (getContainerFocused()->y +
//               getContainerFocused()->get_selector(z)->y + 14))) {
//       toreturn = getContainerFocused()->get_selector(z);
//     }
//   }
//   return toreturn;
// }

// void ui_manager::onKeyDown(SDL_Event e) {
//   auto keySym = e.key.keysym.sym;
//
//   if (!blocked) {
//     if (keySym == SDLK_BACKSPACE) {
//       getInputBoxFocused()->remove_last();
//     } else if (keySym == SDLK_TAB) {
//       changeInputBoxFocus(getInputBoxFocused()->index + 1);
//     } else if (getInputBoxFocused() != nullptr) {
//       const std::set<SDL_Keycode> allowedSpecialChars = {
//           SDLK_SPACE,     SDLK_PERIOD,     SDLK_EXCLAIM,
//           SDLK_QUESTION,  SDLK_COLON,      SDLK_MINUS,
//           SDLK_LEFTPAREN, SDLK_RIGHTPAREN, SDLK_EQUALS};
//
//       // If the character is allowed, write it
//       if (allowedSpecialChars.find(keySym) != allowedSpecialChars.end()) {
//         getInputBoxFocused()->write(*SDL_GetKeyName(keySym));
//       }
//
//       if ((keySym >= SDLK_0 && keySym <= SDLK_9) ||
//           (keySym >= SDLK_a && keySym <= SDLK_z)) {
//         if (writing) {
//           getInputBoxFocused()->write(*SDL_GetKeyName(keySym));
//         }
//       }
//     }
//   }
// }

}  // namespace ui
}  // namespace bure

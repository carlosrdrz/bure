#include "container.h"

using namespace std::placeholders;

namespace bure {
namespace ui {

container::container() {
    this->parent = nullptr;

    this->buttonPressed = false;
    this->focused = false;
    this->index = 0;
    this->visible = true;

    bure::event_manager::get().addEventCallback(
        SDL_MOUSEBUTTONUP, std::bind(&container::onClickUp, this, _1));
}

container::container(int x, int y, int w, int h) {
    this->parent = nullptr;

    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

    this->focused = false;
    this->index = 0;
    this->visible = true;
    this->buttonPressed = false;

    bure::event_manager::get().addEventCallback(
        SDL_MOUSEBUTTONUP, std::bind(&container::onClickUp, this, _1));
}

container::~container() {
    labels.clear();
    inputBoxes.clear();
    buttons.clear();
    images.clear();
}

void container::add(std::unique_ptr<label> w) {
    w->index = static_cast<int>(labels.size());
    // w->setParent(this);
    labels.push_back(std::move(w));
}

void container::add(std::unique_ptr<button> w) {
    w->index = static_cast<int>(buttons.size());
    // w->setParent(this);
    buttons.push_back(std::move(w));
}

void container::add(std::unique_ptr<input_box> w) {
    w->index = static_cast<int>(inputBoxes.size());
    // w->setParent(this);
    inputBoxes.push_back(std::move(w));
    if (inputBoxes.size() == 1) {
        // inputBoxOnFocus = inputBoxes.at(0).get();
        w->focused = true;
    }
}

void container::add(std::unique_ptr<image> w) {
    w->index = static_cast<int>(images.size());
    images.push_back(std::move(w));
}

void container::add(std::unique_ptr<selector> w) {
    w->index = static_cast<int>(selectors.size());
    selectors.push_back(std::move(w));
}

// void container::changeButtonFocus(int num) {
//     for (auto&& button : buttons) {
//         if (button->index == num) {
//             buttonOnFocus = button.get();
//             break;
//         }
//     }
// }

// void container::changeIBFocus(int num) {
//     for (auto&& ib : inputBoxes) {
//         if (ib->index == num) {
//             inputBoxOnFocus = ib.get();
//             break;
//         }
//     }
// }

input_box* container::get_input_box(int id) {
    if (id > static_cast<int>(inputBoxes.size()) - 1) return nullptr;

    for (auto&& e : inputBoxes) {
        if (e->index == id) {
            return e.get();
        }
    }

    return nullptr;
}

label* container::get_label(int id) {
    if (id > static_cast<int>(labels.size()) - 1) return nullptr;

    for (auto&& e : labels) {
        if (e->index == id) {
            return e.get();
        }
    }

    return nullptr;
}

button* container::get_button(int id) {
    if (id > static_cast<int>(buttons.size()) - 1) return nullptr;

    for (auto&& e : buttons) {
        if (e->index == id) {
            return e.get();
        }
    }

    return nullptr;
}

image* container::get_image(int id) {
    if (id > static_cast<int>(images.size()) - 1) return nullptr;

    for (auto&& e : images) {
        if (e->index == id) {
            return e.get();
        }
    }

    return nullptr;
}

selector* container::get_selector(int id) {
    if (id > static_cast<int>(selectors.size()) - 1) return nullptr;

    for (auto&& e : selectors) {
        if (e->index == id) {
            return e.get();
        }
    }

    return nullptr;
}

void container::onClickUp(SDL_Event e) {
  int x = e.button.x / 2;
  int y = e.button.y / 2;
  focused = wasClicked(x, y);
}


}  // namespace ui
}  // namespace bure

#include "container_script.h"

#include <list>

namespace bure {
namespace ui {

container::container() {
    this->buttonPressed = false;
    this->focused = false;
    this->index = 0;
    this->visible = true;
}

container::container(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

    this->focused = false;
    this->index = 0;
    this->visible = true;
    this->buttonPressed = false;
}

container::~container() {
    labels.clear();
    inputBoxes.clear();
    buttons.clear();
    images.clear();
}

void container::add(label w) {
    w.index = static_cast<int>(labels.size());
    labels.push_back(w);
}

void container::add(button w) {
    w.index = static_cast<int>(buttons.size());
    buttons.push_back(w);
}

void container::add(input_box w) {
    w.index = static_cast<int>(inputBoxes.size());
    inputBoxes.push_back(w);
    if (inputBoxes.size() == 1) {
        inputBoxOnFocus = &(*(inputBoxes.begin()));
        inputBoxOnFocus->focused = true;
    }

}

void container::add(image w) {
    w.index = static_cast<int>(images.size());
    images.push_back(w);
}

void container::add(selector w) {
    w.index = static_cast<int>(selectors.size());
    selectors.push_back(w);
}

void container::changeButtonFocus(int num) {
    for (auto& button : buttons) {
        if (button.index == num) {
            buttonOnFocus = &button;
            break;
        }
    }
}

void container::changeIBFocus(int num) {
    for (auto& ib : inputBoxes) {
        if (ib.index == num) {
            inputBoxOnFocus = &ib;
            break;
        }
    }
}

input_box *container::get_input_box(int id) {
    if (id > static_cast<int>(inputBoxes.size()) - 1) return nullptr;

    for (auto &e : inputBoxes) {
        if (e.index == id) {
            return &e;
        }
    }

    return nullptr;
}

label *container::get_label(int id) {
    if (id > static_cast<int>(labels.size()) - 1) return nullptr;

    for (auto &e : labels) {
        if (e.index == id) {
            return &e;
        }
    }

    return nullptr;
}

button *container::get_button(int id) {
    if (id > static_cast<int>(buttons.size()) - 1) return nullptr;

    for (auto &e : buttons) {
        if (e.index == id) {
            return &e;
        }
    }

    return nullptr;
}

image *container::get_image(int id) {
    if (id > static_cast<int>(images.size()) - 1) return nullptr;

    for (auto &e : images) {
        if (e.index == id) {
            return &e;
        }
    }

    return nullptr;
}

selector *container::get_selector(int id) {
    if (id > static_cast<int>(selectors.size()) - 1) return nullptr;

    for (auto &e : selectors) {
        if (e.index == id) {
            return &e;
        }
    }

    return nullptr;
}
}  // namespace ui
}  // namespace bure

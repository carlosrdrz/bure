#include "container.h"

#include <list>

ui::container::container() {
    this->buttonPressed = false;
    this->focused = false;
    this->index = 0;
    this->visible = true;
}

ui::container::container(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

    this->focused = false;
    this->index = 0;
    this->visible = true;
    this->buttonPressed = false;
}

ui::container::~container() {
    labels.clear();
    inputBoxes.clear();
    buttons.clear();
    images.clear();
}

void ui::container::add(label w) {
    w.index = static_cast<int>(labels.size());
    labels.push_back(w);
}

void ui::container::add(button w) {
    w.index = static_cast<int>(buttons.size());
    buttons.push_back(w);
}

void ui::container::add(input_box w) {
    w.index = static_cast<int>(inputBoxes.size());
    inputBoxes.push_back(w);
    if (inputBoxes.size() == 1) {
        inputBoxOnFocus = &(*(inputBoxes.begin()));
        inputBoxOnFocus->focused = true;
    }

}

void ui::container::add(image w) {
    w.index = static_cast<int>(images.size());
    images.push_back(w);
}

void ui::container::add(selector w) {
    w.index = static_cast<int>(selectors.size());
    selectors.push_back(w);
}

void ui::container::changeButtonFocus(int num) {
    for (auto& button : buttons) {
        if (button.index == num) {
            buttonOnFocus = &button;
            break;
        }
    }
}

void ui::container::changeIBFocus(int num) {
    for (auto& ib : inputBoxes) {
        if (ib.index == num) {
            inputBoxOnFocus = &ib;
            break;
        }
    }
}

ui::input_box *ui::container::get_input_box(int id) {
    if (id > static_cast<int>(inputBoxes.size()) - 1) return nullptr;

    for (auto &e : inputBoxes) {
        if (e.index == id) {
            return &e;
        }
    }

    return nullptr;
}

ui::label *ui::container::get_label(int id) {
    if (id > static_cast<int>(labels.size()) - 1) return nullptr;

    for (auto &e : labels) {
        if (e.index == id) {
            return &e;
        }
    }

    return nullptr;
}

ui::button *ui::container::get_button(int id) {
    if (id > static_cast<int>(buttons.size()) - 1) return nullptr;

    for (auto &e : buttons) {
        if (e.index == id) {
            return &e;
        }
    }

    return nullptr;
}

ui::image *ui::container::get_image(int id) {
    if (id > static_cast<int>(images.size()) - 1) return nullptr;

    for (auto &e : images) {
        if (e.index == id) {
            return &e;
        }
    }

    return nullptr;
}

ui::selector *ui::container::get_selector(int id) {
    if (id > static_cast<int>(selectors.size()) - 1) return nullptr;

    for (auto &e : selectors) {
        if (e.index == id) {
            return &e;
        }
    }

    return nullptr;
}

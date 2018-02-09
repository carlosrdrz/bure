#include "label.h"

ui::label::label(std::string title, int size) {
    this->text = title;
    this->size = size;
}

void ui::label::set(int x, int y) {
    this->x = x;
    this->y = y;
}

void ui::label::set_text(std::string value) {
    this->text = value;
}

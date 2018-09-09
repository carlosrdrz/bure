#include "label.h"

namespace bure {
namespace ui {

label::label(std::string title, int size) {
    this->text = title;
    this->size = size;
}

void label::set(int x, int y) {
    this->x = x;
    this->y = y;
}

void label::setText(std::string value) {
    this->text = value;
}

}  // namespace ui
}  // namespace bure

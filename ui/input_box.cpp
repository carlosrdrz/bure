#include "input_box.h"

namespace bure {
namespace ui {

input_box::input_box() {
    this->index = -1;
    this->max_length = 12;
    this->function = nullptr;
}

input_box::input_box(std::string title, std::string text, int max_length) {
    this->title = title;
    this->text = text;
    this->index = -1;
    this->max_length = max_length;
    this->function = nullptr;
}

void input_box::set(int x, int y, int w) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = 14;
}

void input_box::remove_last() {
    text.pop_back();
}

void input_box::write(char que) {
    if (current_length() >= max_length) return;
    text.push_back(que);
}

}  // namespace ui
}  // namespace bure

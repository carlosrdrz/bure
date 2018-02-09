#include "input_box.h"

ui::input_box::input_box() {
    this->index = -1;
    this->max_length = 12;
    this->function = nullptr;
}

ui::input_box::input_box(std::string title, std::string text, int max_length) {
    this->title = title;
    this->text = text;
    this->index = -1;
    this->max_length = max_length;
    this->function = nullptr;
}

void ui::input_box::set(int x, int y, int w) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = 14;
}

void ui::input_box::remove_last() {
    text.pop_back();
}

void ui::input_box::write(char que) {
    if (current_length() >= max_length) return;
    text.push_back(que);
}

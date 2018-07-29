#include "input_box.h"

using namespace std::placeholders;

namespace bure {
namespace ui {

input_box::input_box() {
    this->index = -1;
    this->max_length = 12;
    this->function = nullptr;

    bure::event_manager::get().addEventCallback(
        SDL_MOUSEBUTTONUP, std::bind(&input_box::onClickUp, this, _1));
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

void input_box::onClickUp(SDL_Event e) {
  int x = e.button.x / 2;
  int y = e.button.y / 2;
  focused = wasClicked(x, y);
}


}  // namespace ui
}  // namespace bure

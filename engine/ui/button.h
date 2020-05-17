#pragma once

#include <functional>
#include <string>

#include "widget.h"

namespace bure {
namespace ui {

class button : public widget {
 public:
  using callback = std::function<void(int)>;

  std::string title;
  callback function;
  int press;
  int parameter;

  button();
  explicit button(std::string t);

  void init() override;
  void deinit() override;

  std::string getTitle() const { return title; }

  void set(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;
  }

  widget_type getType() const override { return widget_type::button; }

 private:
  cb_handler _down_handler, _up_handler;
  void onClickDown(SDL_Event e);
  void onClickUp(SDL_Event e);
};

}  // namespace ui
}  // namespace bure

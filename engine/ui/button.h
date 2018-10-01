#pragma once

#include <string>
#include <functional>

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

    std::string getTitle() const { return title; }
    void init();

    void set(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
    }

    widget_type getType() const { return widget_type::button; }

 private:
    void onClickDown(SDL_Event e);
    void onClickUp(SDL_Event e);
};

}  // namespace ui
}  // namespace bure

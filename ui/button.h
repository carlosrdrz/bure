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
    ~button() = default;

    const char *get_title() { return title.c_str(); }

    void set(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
    }

 private:
    void onClickDown(SDL_Event e);
    void onClickUp(SDL_Event e);
};

}  // namespace ui
}  // namespace bure

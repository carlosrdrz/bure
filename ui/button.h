#pragma once

#include "widget.h"

#include <string>
#include <functional>

namespace ui {
class button : public widget {
 public:
    using callback = std::function<void(int)>;

    std::string title;
    callback function;
    int press;
    int parameter;

    button() : press{0}, parameter{0} {}
    explicit button(std::string t) : title(t), press{0}, parameter{0} {}
    ~button() = default;

    const char *get_title() { return title.c_str(); }

    void set(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
    }
};
};  // namespace ui

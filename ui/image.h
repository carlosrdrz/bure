#pragma once

#include "widget.h"

#include <SDL_image.h>
#include <string>

namespace ui {
class image : public widget {
 public:
    SDL_Surface *img;

    image() : img(nullptr) {}
    explicit image(std::string path) {
        img = IMG_Load(path.c_str());
    }

    ~image() { SDL_FreeSurface(img); }

    void set(int x, int y) {
        this->x = x;
        this->y = y;
    }
};
};  // namespace ui

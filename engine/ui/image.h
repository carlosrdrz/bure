#pragma once

#include <string>

#include "SDL_image.h"
#include "widget.h"

namespace bure {
namespace ui {

class image : public widget {
 public:
  SDL_Surface *img;

  image() : img(nullptr) {}
  explicit image(std::string path) { img = IMG_Load(path.c_str()); }

  widget_type getType() const override { return widget_type::image; }
  ~image() { SDL_FreeSurface(img); }

  void set(int x, int y) {
    this->x = x;
    this->y = y;
  }
};

}  // namespace ui
}  // namespace bure

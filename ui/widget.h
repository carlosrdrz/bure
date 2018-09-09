#pragma once

#include "../engine/event_manager.h"

namespace bure {
namespace ui {

class widget {
 public:
  widget() : width(0), height(0), x(0), y(0), index(0), parent(nullptr) {}

  int getRelativeX() const;
  int getRelativeY() const;
  int getAbsoluteX() const;
  int getAbsoluteY() const;
  int getWidth() const;
  int getHeight() const;

  bool wasClicked(int x, int y) const;

  void setParent(widget* w);
  widget* getParent() const;

 protected:
  int width, height, x, y, index;
  widget* parent = nullptr;
};

}  // namespace ui
}  // namespace bure

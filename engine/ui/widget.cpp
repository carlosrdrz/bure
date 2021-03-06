#include "widget.h"

#include <cassert>

#include "utils/logger.h"

namespace bure {
namespace ui {

int widget::getRelativeX() const { return x; }

int widget::getRelativeY() const { return y; }

int widget::getAbsoluteX() const {
  if (parent == nullptr) return x;
  return parent->getAbsoluteX() + x;
}

int widget::getAbsoluteY() const {
  if (parent == nullptr) return y;
  return parent->getAbsoluteY() + y;
}

int widget::getWidth() const { return width; }

int widget::getHeight() const { return height; }

bool widget::wasClicked(int clickX, int clickY) const {
  auto x = getAbsoluteX();
  auto y = getAbsoluteY();

  return (clickX > x && clickX < (x + width) && clickY > y &&
          clickY < (y + height));
}

void widget::setParent(widget* w) { parent = w; }

widget* widget::getParent() const { return parent; }

widget_type widget::getType() const {
  assert(false);
  return widget_type::widget;
}

void widget::setLayer(int layer) { _layer = layer; }

int widget::getLayer() {
  if (parent == nullptr || _layer != 0) {
    return _layer;
  } else {
    return parent->getLayer();
  }
}

}  // namespace ui
}  // namespace bure

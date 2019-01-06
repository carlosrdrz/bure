#include "container.h"

using namespace std::placeholders;

namespace bure {
namespace ui {

container::container(int x, int y, int w, int h) {
  this->x = x;
  this->y = y;
  this->width = w;
  this->height = h;
}

void container::deinit() {
  for (auto& cc : _children) {
    cc.child->deinit();
  }
}

void container::add(std::unique_ptr<widget> w) {
  w->setParent(this);
  w->init();
  auto type = w->getType();
  _children.push_back({std::move(w), type});
}

std::vector<std::reference_wrapper<widget>> container::getChildrenByType(
    widget_type type) {
  std::vector<std::reference_wrapper<widget>> ret;

  for (auto& c : _children) {
    if (c.type == type) {
      ret.emplace_back(std::ref(*c.child));
    }
  }

  return ret;
}

}  // namespace ui
}  // namespace bure

#pragma once

#include <memory>
#include <vector>

#include "button.h"
#include "image.h"
#include "input_box.h"
#include "label.h"
#include "selector.h"
#include "widget.h"

namespace bure {
namespace ui {

struct container_child {
  std::unique_ptr<widget> child;
  widget_type type;
};

class container : public widget {
 public:
  container(int x, int y, int w, int h);

  void add(std::unique_ptr<widget> w);

  widget_type getType() const { return widget_type::container; }
  std::vector<std::reference_wrapper<widget>> getChildrenByType(
      widget_type type);

 private:
  std::vector<container_child> _children;
};

}  // namespace ui
}  // namespace bure

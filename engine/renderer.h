#pragma once

#include <memory>

#include "graphics.h"

namespace bure {

class renderer {
 public:
  void init(std::shared_ptr<bure::graphics> gr);
  virtual void render(int layer) {}

 protected:
  std::shared_ptr<bure::graphics> _graphics;
};

}  // namespace bure

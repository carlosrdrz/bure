#pragma once

#include <memory>

#include "graphics2.h"

namespace bure {

class renderer {
 public:
  void init(std::shared_ptr<bure::graphics2> gr);
  virtual void render() {}

 protected:
  std::shared_ptr<bure::graphics2> _graphics;
};

}  // namespace bure

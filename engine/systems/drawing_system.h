#pragma once

#include <memory>

#include "system.h"

namespace bure {
namespace systems {

class drawing_system : public system {
 public:
  void init() override;
  void update() override;

 private:
  std::unique_ptr<bure::graphics2> graphics;
};

}  // namespace systems
}  // namespace bure

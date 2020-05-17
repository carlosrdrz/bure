#pragma once

#include <memory>
#include <vector>

#include "../renderer.h"
#include "system.h"

namespace bure {
namespace systems {

class drawing_system : public system {
 public:
  explicit drawing_system(std::unique_ptr<bure::graphics> gr);

  void init() override;
  void update() override;

  void addRenderer(std::unique_ptr<bure::renderer> rend);

 private:
  std::vector<std::unique_ptr<bure::renderer>> _renderers;
  std::shared_ptr<bure::graphics> _graphics;
};

}  // namespace systems
}  // namespace bure

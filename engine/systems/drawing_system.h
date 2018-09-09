#pragma once

#include <memory>
#include <vector>

#include "system.h"
#include "../renderer.h"

namespace bure {
namespace systems {

class drawing_system : public system {
 public:
  explicit drawing_system(std::unique_ptr<bure::graphics2> gr);

  void init() override;
  void update() override;

  void addRenderer(std::unique_ptr<bure::renderer> rend);

 private:
  std::vector<std::unique_ptr<bure::renderer>> _renderers;
  std::shared_ptr<bure::graphics2> _graphics;
};

}  // namespace systems
}  // namespace bure

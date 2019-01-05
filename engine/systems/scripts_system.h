#pragma once

#include "system.h"

namespace bure {
namespace systems {

class scripts_system : public system {
 public:
  void update() override;
};

}  // namespace systems
}  // namespace bure

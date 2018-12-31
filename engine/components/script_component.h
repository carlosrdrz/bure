#pragma once

#include "component.h"

namespace bure {
namespace components {

class script_component : public component {
 using component::component;

 public:
  virtual void onTick() {}
};

}  // namespace components
}  // namespace bure

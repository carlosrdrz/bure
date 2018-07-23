#pragma once

#include "../entities/entity.h"

namespace bure {

namespace entities {
class entity;
}  // namespace entities

namespace components {

class component {
 public:
  explicit component(const bure::entities::entity& e) : _entity(e) {}

  virtual void update() {}
  virtual void destroy() {}

 protected:
  const bure::entities::entity& _entity;
};

}  // namespace components
}  // namespace bure
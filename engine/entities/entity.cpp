#include "entity.h"

namespace bure {
namespace entities {

void entity::destroy() {
  if (!_parent) return;
  _parent->removeChild(this);
}

entity* entity::getParent() const { return _parent; }

std::list<entity*> entity::getChildren() const {
  std::list<entity*> ptrList;
  for (auto& child : _children) {
    ptrList.push_back(child.get());
  }
  return ptrList;
}

void entity::addChild(std::unique_ptr<entity> e) {
  _children.push_back(std::move(e));
}

void entity::removeChild(entity* e) {
  for (auto& child : _children) {
    if (e == child.get()) _children.remove(child);
  }
}

void entity::setLayer(int layer) { _layer = layer; }

int entity::getLayer() {
  if (_parent == nullptr || _layer != 0) {
    return _layer;
  } else {
    return _parent->getLayer();
  }
}

}  // namespace entities
}  // namespace bure

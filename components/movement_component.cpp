#include "movement_component.h"

void movement_component::setPosition(bure::map_coords mc) {
  _currentPosition = mc;
}

void movement_component::setVelocity(int v) { _velocity = v; }
void movement_component::setState(movement_state s) { _state = s; }

bure::map_coords movement_component::getPosition() { return _currentPosition; }
int movement_component::getVelocity() { return _velocity; }
movement_state movement_component::getState() { return _state; }

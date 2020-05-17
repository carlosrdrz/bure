#include "movement_component.h"

void movement_component::setVelocity(int v) { _velocity = v; }
void movement_component::setDirection(bure::direction d) { _direction = d; }

int movement_component::getVelocity() { return _velocity; }
bure::direction movement_component::getDirection() { return _direction; }

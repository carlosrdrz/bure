#include "stats_component.h"

void stats_component::setHP(int hp) { _hp = hp; }
void stats_component::setMaxHP(int hp) { _max_hp = hp; }

int stats_component::getHP() { return _hp; }
int stats_component::getMaxHP() { return _max_hp; }

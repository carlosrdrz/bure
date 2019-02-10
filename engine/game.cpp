#include "game.h"

namespace bure {

void game::finish() { _finished = true; }

bool game::isFinished() { return _finished; }

}  // namespace bure

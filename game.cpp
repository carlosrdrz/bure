#include "game.h"
#include "engine/utils/logger.h"

game::game()
    : finished(false), playing(false), _player(std::make_unique<player>()) {}

void game::finishGame() {
  finished = true;
  playing = false;
}

void game::nextFrame() { _player->nextWalkingFrame(); }

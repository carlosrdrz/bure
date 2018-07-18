#include "game.h"

game::game()
    : finished(false), playing(false), _player(std::make_unique<player>()) {}

void game::finishGame() {
  finished = true;
  playing = false;
}

void game::changeMap(std::string mapName) {
  // todo: fix base path
  _map = make_unique<bure::game_map>("/home/carlosrdrz/dev/bure", mapName);
}

void game::nextFrame() { _player->nextWalkingFrame(); }

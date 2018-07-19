#pragma once

#include <list>
#include <memory>
#include <string>

#include "engine/game_map.h"
#include "player.h"

class game {public:
  game();

  // Set to true when the player signals that he wants the
  // game to be finished by closing the window or sending some
  // command.
  bool finished;

  // Set to true when the player is actually playing the game
  // (instead of in the start menu, for instance).
  bool playing;

  // Finishes the actual game and closes the client
  void finishGame();

  // Map
  bure::game_map *getMap() { return _map.get(); }
  void changeMap(std::string mapName);

  // Player
  player &getPlayer() { return *_player; }

  // Other
  void nextFrame();

 private:
  std::unique_ptr<player> _player;
  std::unique_ptr<bure::game_map> _map;
};

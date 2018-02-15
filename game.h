#ifndef __H_GAME__
#define __H_GAME__

#include "player.h"
#include "game_map.h"
#include "error.h"

#include <list>
#include <string>

class game {
 public:
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
    game_map* getMap() { return _map.get(); }
    void changeMap(std::string mapName);

    // Player
    player& getPlayer() { return *_player; }

    // Other
    void nextFrame();

 private:
    std::unique_ptr<player> _player;
    std::unique_ptr<game_map> _map;
};
#endif

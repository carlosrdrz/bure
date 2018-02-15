#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "user_interface.h"
#include "game.h"
#include "config.h"

#include <SDL.h>
#include <set>

class event_manager {
 public:
    event_manager(game &g, user_interface &u) : _game(g), _ui(u) {}
    ~event_manager() = default;

    int poll_event();
    void process_event();

 private:
    game& _game;
    user_interface& _ui;

    SDL_Event lastEvent;
};


#endif // EVENT_MANAGER_H

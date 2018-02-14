#ifndef BURE_EVENT_MANAGER_H
#define BURE_EVENT_MANAGER_H

#include "user_interface.h"
#include "game.h"
#include "config.h"

#include <SDL.h>
#include <set>

class event_manager {
 public:
    event_manager(game &g, player &p, user_interface &u) : gameInstance(g), playerInstance(p), userInterface(u) {}
    ~event_manager() = default;

    int poll_event();
    void process_event();

 private:
    game& gameInstance;
    user_interface& userInterface;
    player& playerInstance;

    SDL_Event lastEvent;
};


#endif //BURE_EVENT_MANAGER_H

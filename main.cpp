#include <vector>
#include <set>
#include <memory>

#include "graphics.h"
#include "config.h"
#include "event_manager.h"

// Main objects
graphics *pantalla;
player *playerInstance;
game *gameInstance;

config config::instance;

int main(int argc, char *argv[]) {
    atexit(SDL_Quit);

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Error::Log(SDL_GetError(), 3);
    }

    // Init objects
    config::instance.readFile(argv[1]);

    auto graphicsUniquePtr = std::make_unique<graphics>(argv[1]);
    pantalla = graphicsUniquePtr.get();

    auto userInterface = std::make_unique<user_interface>();
    auto playerUniquePtr = std::make_unique<player>();

    // Ignoring these for now, so hardcoding them here
    playerInstance = playerUniquePtr.get();
    playerInstance->nombre = "Carlos";
    playerInstance->hp = playerInstance->mp = playerInstance->nivel = 100;
    playerInstance->traje = playerInstance->clase = 1;

    auto gameUniquePtr = std::make_unique<game>();
    gameInstance = gameUniquePtr.get();

    auto eventManager = std::make_unique<event_manager>(
        *gameUniquePtr, *playerUniquePtr, *userInterface);

    userInterface->changeInputBoxFocus(0);

    while (gameInstance->abierto) {
        if (gameInstance->playing) {
            pantalla->clean();
            pantalla->draw(gameInstance->currentMap, false);
            pantalla->drawPlayers();
            pantalla->draw(gameInstance->currentMap, true);
            gameInstance->nextAnimationFrame();
        } else {
            pantalla->renderBackground();
        }

        pantalla->draw(userInterface.get());
        pantalla->flipBuffer();

        while (eventManager->poll_event()) eventManager->process_event();

        SDL_Delay(10);
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}

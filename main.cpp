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
    config::instance.readFile(argv[1]);

    auto graphicsUniquePtr = std::make_unique<graphics>(argv[1]);
    pantalla = graphicsUniquePtr.get();

    auto userInterface = std::make_unique<user_interface>();

    auto playerUniquePtr = std::make_unique<player>();
    playerInstance = playerUniquePtr.get();

    auto gameUniquePtr = std::make_unique<game>();
    gameInstance = gameUniquePtr.get();

    auto eventManager = std::make_unique<event_manager>(
        *gameUniquePtr, *playerUniquePtr, *userInterface);

    while (gameInstance->abierto) {
        if (gameInstance->playing) {
            pantalla->clean();
            pantalla->draw(gameInstance->currentMap, false);
            pantalla->drawPlayer();
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

#include <vector>
#include <set>
#include <memory>

#include "graphics.h"
#include "config.h"
#include "event_manager.h"

config config::instance;

void buildStartMenu(game* g, user_interface* ui) {
    auto c = new ui::container(362, 309, 130, 60);
    auto b = ui::button("START GAME");
    b.set(20, 20, 90, 20);
    b.function = [g, ui](int) -> void {
        g->changeMap("campo.tmx");
        g->getPlayer().setPosition(30, 36);
        g->playing = true;
        ui->writing = false;
        ui->closeContainer(0);
    };

    c->add(b);
    ui->addContainer(c);
}

int main(int argc, char *argv[]) {
    config::instance.readFile(argv[1]);

    // Init main objects
    auto graphicsInstance = std::make_unique<graphics>(argv[1]);
    auto gameInstance = std::make_unique<game>();
    auto userInterface = std::make_unique<user_interface>();
    auto eventManager = std::make_unique<event_manager>(
            *gameInstance, *userInterface);

    // Init start menu
    buildStartMenu(gameInstance.get(), userInterface.get());

    // Main game loop
    while (!gameInstance->finished) {
        if (gameInstance->playing) {
            graphicsInstance->clean();
            graphicsInstance->draw(*gameInstance);
            gameInstance->nextFrame();
        } else {
            graphicsInstance->renderBackground();
        }

        graphicsInstance->draw(userInterface.get());
        graphicsInstance->flipBuffer();

        while (eventManager->poll_event()) eventManager->process_event();

        SDL_Delay(10);
    }

    return EXIT_SUCCESS;
}

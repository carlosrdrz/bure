#include <vector>
#include <set>
#include <memory>

#include "SDL.h"
#include "graphics.h"
#include "game.h"
#include "error.h"
#include "config.h"

// Threads
SDL_Event event;

// Main objects
graphics *pantalla;
user_interface *uiInstance;
player *playerInstance;
game *gameInstance;
config *configInstance;
std::string path;

// Funciones de eventos
void manageEvent();

int main(int argc, char *argv[]) {
    atexit(SDL_Quit);

    // Copy local resources from command line
    path = argv[1];

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Error::Log(SDL_GetError(), 3);
    }

    // Init objects
    auto configUniquePtr = std::make_unique<config>();
    configInstance = configUniquePtr.get();

    auto graphicsUniquePtr = std::make_unique<graphics>();
    pantalla = graphicsUniquePtr.get();

    auto userInterfaceUniquePtr = std::make_unique<user_interface>();
    uiInstance = userInterfaceUniquePtr.get();

    auto playerUniquePtr = std::make_unique<player>();

    // Ignoring these for now, so hardcoding them here
    playerInstance = playerUniquePtr.get();
    playerInstance->nombre = "Carlos";
    playerInstance->hp = playerInstance->mp = playerInstance->nivel = 100;
    playerInstance->traje = playerInstance->clase = 1;

    auto gameUniquePtr = std::make_unique<game>();
    gameInstance = gameUniquePtr.get();

    uiInstance->changeInputBoxFocus(0);

    while (gameInstance->abierto) {
        if (gameInstance->playing) {
            pantalla->clean();
            pantalla->draw(gameInstance->currentMap, false);
            pantalla->drawPlayers();
            pantalla->draw(gameInstance->currentMap, true);
            gameInstance->MensajesLoop();
            gameInstance->nextAnimationFrame();
        } else {
            pantalla->renderBackground();
        }

        pantalla->draw(uiInstance);
        pantalla->flipBuffer();

        while (SDL_PollEvent(&event)) manageEvent();

        SDL_Delay(10);
    }

    SDL_Quit();

    return 0;
}

void manageEvent() {
    int x = event.button.x / 2;
    int y = event.button.y / 2;

    switch (event.type) {
        case SDL_QUIT:
            gameInstance->cerrar();
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (!uiInstance->blocked && uiInstance->writing) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (uiInstance->clickOnContainer(x, y)) {
                        if (!uiInstance->getContainerClicked(x, y)->focused)
                            uiInstance->changeContainerFocus(uiInstance->getContainerClicked(x, y)->index);
                    }

                    if (uiInstance->clickOnInputBox(x, y)) {
                        uiInstance->changeInputBoxFocus(uiInstance->getInputBoxClicked(x, y)->index);
                    } else if (uiInstance->clickOnButton(x, y)) {
                        uiInstance->changeButtonFocus(uiInstance->getButtonClicked(x, y)->index);
                        uiInstance->getButtonFocused()->press = 1;
                        uiInstance->getContainerClicked(x, y)->buttonPressed = true;
                    } else if (uiInstance->clickOnSelector(x, y)) {
                        if (x > (uiInstance->getContainerFocused()->x + uiInstance->getSelectorClicked(x, y)->x) &&
                            x < (uiInstance->getContainerFocused()->x
                                 + uiInstance->getSelectorClicked(x, y)->x + 9) &&
                            y > (uiInstance->getContainerFocused()->y + uiInstance->getSelectorClicked(x, y)->y)
                            && y < (uiInstance->getContainerFocused()->y + uiInstance->getSelectorClicked(x, y)->y + 14)) {
                            uiInstance->getSelectorClicked(x, y)->previous();
                        } else {
                            uiInstance->getSelectorClicked(x, y)->next();
                        }
                    }
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (!uiInstance->blocked && uiInstance->writing && uiInstance->getContainerFocused() != nullptr) {
                if (uiInstance->getContainerFocused()->buttonPressed) {
                    uiInstance->getButtonFocused()->press = 0;
                    uiInstance->getContainerFocused()->buttonPressed = false;
                    if (uiInstance->clickOnButton(x, y)) {
                        if (uiInstance->getButtonClicked(x, y) == uiInstance->getButtonFocused())
                            uiInstance->execButton(uiInstance->getButtonClicked(x, y));
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (!uiInstance->blocked) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    gameInstance->cerrar();
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    uiInstance->getInputBoxFocused()->remove_last();
                } else if (event.key.keysym.sym == SDLK_TAB) {
                    // Se focusea al input box siguiente en la std::lista
                    uiInstance->changeInputBoxFocus(uiInstance->getInputBoxFocused()->index + 1);
                } else if (event.key.keysym.sym == SDLK_F12) {
                    static ui::container *id = nullptr;
                    if (uiInstance->containerExists(id)) {
                        if (id->get_selector(2)->get_selected() == "SI") {
                            configInstance->setValueOf("fullscreen", "1");
                        } else {
                            configInstance->setValueOf("fullscreen", "0");
                        }
                        uiInstance->closeContainer(id);
                    } else {
                        // SI NO ESTA CREADO CONSTRUIR MENU OPCIONES
                        auto* settingsContainer = new ui::container(362, 180, 300, 100);
                        auto fullscreen = ui::label("PANTALLA COMPLETA", 8);
                        fullscreen.set(35, 65);
                        auto fullScreenSelector = ui::selector(220, 65);
                        if (configInstance->getBoolValueOf("fullscreen")) {
                            fullScreenSelector.add_option("SI");
                            fullScreenSelector.add_option("NO");
                        } else {
                            fullScreenSelector.add_option("NO");
                            fullScreenSelector.add_option("SI");
                        }
                        settingsContainer->add(fullscreen);
                        settingsContainer->add(fullScreenSelector);
                        uiInstance->addContainer(settingsContainer);
                        id = settingsContainer;
                        uiInstance->blocked = false;
                        uiInstance->writing = true;
                    }
                } else if (gameInstance->playing && !uiInstance->writing) {
                    if (event.key.keysym.sym == SDLK_a && playerInstance->x > 0) {
                        playerInstance->moveLeft();
                    } else if (event.key.keysym.sym == SDLK_s && playerInstance->y < gameInstance->currentMap->height - 1) {
                        playerInstance->moveDown();
                    } else if (event.key.keysym.sym == SDLK_d && playerInstance->x < gameInstance->currentMap->width - 1) {
                        playerInstance->moveRight();
                    } else if (event.key.keysym.sym == SDLK_w && playerInstance->y > 0) {
                        playerInstance->moveUp();
                    }
                } else if (uiInstance->getInputBoxFocused() != nullptr) {
                    if (true) {
                    // todo: fix this
                    // if (uiInstance->getInputBoxFocused()->escapeable()) {
                        const std::set<SDL_Keycode> allowedSpecialChars = {
                                SDLK_SPACE, SDLK_PERIOD, SDLK_EXCLAIM, SDLK_QUESTION, SDLK_COLON,
                                SDLK_MINUS, SDLK_LEFTPAREN, SDLK_RIGHTPAREN, SDLK_EQUALS
                        };

                        // If the character is allowed, write it
                        if (allowedSpecialChars.find(event.key.keysym.sym) != allowedSpecialChars.end()) {
                            uiInstance->getInputBoxFocused()->write(*SDL_GetKeyName(event.key.keysym.sym));
                        }
                    }

                    if ((event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) ||
                        (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z)) {
                        if (uiInstance->writing) {
                            uiInstance->getInputBoxFocused()->write(*SDL_GetKeyName(event.key.keysym.sym));
                        }
                    }
                }
            }

            break;

        default:
            break;
    }
}

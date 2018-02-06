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

// Resources path
#ifdef __LOCAL_RESOURCES__
std::string path = "/home/carlosrdrz/dev/bure/";
#else
std::string path = "/usr/share/bure/";
#endif

// Funciones de eventos
void manageEvent();

int main() {
    atexit(SDL_Quit);

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

    uiInstance->changeIBFocus(0);

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

///////////////////////////////////////////////////
///////////////// HILO DE EVENTOS /////////////////
///////////////////////////////////////////////////

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
                        if (uiInstance->getContainerClicked(x, y)->getFocus() == 0)
                            uiInstance->changeContainerFocus(uiInstance->getContainerClicked(x, y)->index);
                    }

                    if (uiInstance->clickOnIB(x, y)) {
                        uiInstance->changeIBFocus(uiInstance->getInputBoxClicked(x, y)->index);
                    } else if (uiInstance->clickOnButton(x, y)) {
                        uiInstance->changeButtonFocus(uiInstance->getButtonClicked(x, y)->index);
                        uiInstance->getButtonFocused()->press = 1;
                        uiInstance->getContainerClicked(x, y)->boton_pulsado = true;
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
                if (uiInstance->getContainerFocused()->boton_pulsado) {
                    uiInstance->getButtonFocused()->press = 0;
                    uiInstance->getContainerFocused()->boton_pulsado = false;
                    if (uiInstance->clickOnButton(x, y)) {
                        if (uiInstance->getButtonClicked(x, y) == uiInstance->getButtonFocused())
                            uiInstance->ejecutarBoton(uiInstance->getButtonClicked(x, y));
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (!uiInstance->blocked) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    gameInstance->cerrar();
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    uiInstance->getInputBoxFocused()->Borrar();
                } else if (event.key.keysym.sym == SDLK_TAB) {
                    // Se focusea al input box siguiente en la std::lista
                    uiInstance->changeIBFocus(uiInstance->getInputBoxFocused()->index + 1);
                } else if (event.key.keysym.sym == SDLK_F12) {
                    static container *id = nullptr;
                    if (uiInstance->containerExists(id)) {
                        if (id->getSelector(2)->getSelected() == "SI") {
                            configInstance->setValueOf("fullscreen", "1");
                        } else {
                            configInstance->setValueOf("fullscreen", "0");
                        }
                        uiInstance->closeContainer(id);
                    } else {
                        // SI NO ESTA CREADO CONSTRUIR MENU OPCIONES
                        auto* settingsContainer = new container(362, 180, 300, 100);
                        auto* fullscreen = new label("PANTALLA COMPLETA", 8);
                        fullscreen->Set(35, 65);
                        auto* fullScreenSelector = new selector(220, 65);
                        if (configInstance->getBoolValueOf("fullscreen")) {
                            fullScreenSelector->addOption("SI");
                            fullScreenSelector->addOption("NO");
                        } else {
                            fullScreenSelector->addOption("NO");
                            fullScreenSelector->addOption("SI");
                        }
                        settingsContainer->Add(fullscreen);
                        settingsContainer->Add(fullScreenSelector);
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
                    if (uiInstance->getInputBoxFocused()->escapeable()) {
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

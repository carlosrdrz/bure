#include <vector>
#include <set>
#include <memory>

#include "SDL.h"
#include "graphics.h"
#include "network.h"
#include "game.h"
#include "error.h"
#include "config.h"

// Threads
SDL_Thread *sendThread, *receiveThread;
SDL_Event event;

// Main objects
graphics *pantalla;
network *net;
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

// Funciones de threads
int enviar(void *unused) {
    net->sendLoop();
    return 0;
}

int recibir(void *unused) {
    net->recieveLoop();
    return 0;
}

int main() {
    atexit(SDL_Quit);

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Error::Log(SDL_GetError(), 3);
    }

    // SDL_EnableKeyRepeat(400, 100);

    // Init objects
    auto configUniquePtr = std::make_unique<config>();
    configInstance = configUniquePtr.get();

    auto graphicsUniquePtr = std::make_unique<graphics>();
    pantalla = graphicsUniquePtr.get();

    auto userInterfaceUniquePtr = std::make_unique<user_interface>();
    uiInstance = userInterfaceUniquePtr.get();

    auto playerUniquePtr = std::make_unique<player>();
    playerInstance = playerUniquePtr.get();

    auto networkUniquePtr = std::make_unique<network>();
    net = networkUniquePtr.get();

    auto gameUniquePtr = std::make_unique<game>();
    gameInstance = gameUniquePtr.get();

    uiInstance->changeIBFocus(0);

    // Creating sending thread and receiving thread.
    sendThread = SDL_CreateThread(enviar, "sendThread", nullptr);
    receiveThread = SDL_CreateThread(recibir, "receiveThread", nullptr);

    while (gameInstance->abierto) {
        if (gameInstance->princp) {
            gameInstance->princp_parado = false;
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
            if (net->intentoConexion > 0) net->intentoConexion++;
            if (net->intentoConexion > 150) net->traducirPaquete("1_0_NOTSERVER");

            SDL_Delay(10);
        } else {
            gameInstance->princp_parado = true;
            SDL_Delay(100);
        }
    }

    int result = 0;
    // TODO: Let threads know they need to exit
    SDL_WaitThread(sendThread, &result);
    SDL_WaitThread(receiveThread, &result);

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
            if (gameInstance->logged) net->sendPacket("0_0_NOTHING");
            SDL_Delay(250);
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
            if (!uiInstance->blocked && uiInstance->writing && uiInstance->getContainerFocused() != NULL) {
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
                    if (gameInstance->logged) net->sendPacket("0_0_NOTHING");
                    SDL_Delay(250);
                    gameInstance->cerrar();
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    uiInstance->getInputBoxFocused()->Borrar();
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    if (uiInstance->writing) {
                        if (uiInstance->getInputBoxFocused() != NULL &&
                            uiInstance->getInputBoxFocused()->function != NULL) {
                            uiInstance->ejecutarBoton(uiInstance->getInputBoxFocused());
                        }
                    } else if (gameInstance->playing) {
                        // MOSTRAR INPUT BOX PARA ENVIAR MENSAJE
                        // SDL_EnableKeyRepeat(400, 100);

                        container *con = new container(0, 0, 1024, 768);
                        con->SetInvisible();

                        input_box *user = new input_box("Mensaje:", "", 60);
                        user->Set(80, 748, 400);
                        user->function = &user_interface::e_enviarMensaje;
                        user->escapeable(true);
                        con->Add(user);

                        uiInstance->addContainer(con);
                        uiInstance->writing = true;
                    }
                } else if (event.key.keysym.sym == SDLK_TAB) {
                    // Se focusea al input box siguiente en la std::lista
                    uiInstance->changeIBFocus(uiInstance->getInputBoxFocused()->index + 1);
                } else if (event.key.keysym.sym == SDLK_F12) {
                    static container *id = NULL;
                    if (uiInstance->containerExists(id)) {
                        // SI ESTA CREADO GUARDAR OPCIONES Y SALIR
                        if (!id->getSelector(0)->getSelected().compare("SI")) {
                            if (!configInstance->getValueOf("user").compare("0")) {
                                configInstance->setValueOf("user", "User");
                            }
                        } else {
                            configInstance->setValueOf("user", "0");
                        }
                        if (!id->getSelector(1)->getSelected().compare("SI")) {
                            if (!configInstance->getValueOf("pass").compare("0")) {
                                configInstance->setValueOf("pass", "Pass");
                            }
                        } else {
                            configInstance->setValueOf("pass", "0");
                        }
                        if (!id->getSelector(2)->getSelected().compare("SI")) {
                            configInstance->setValueOf("fullscreen", "1");
                        } else {
                            configInstance->setValueOf("fullscreen", "0");
                        }
                        uiInstance->closeContainer(id);
                    } else {
                        // SI NO ESTA CREADO CONSTRUIR MENU OPCIONES
                        container *settingsContainer = new container(362, 180, 300, 100);
                        label *rememberUser = new label("RECORDAR USUARIO", 8);
                        rememberUser->Set(35, 25);
                        label *rememberPassword = new label("RECORDAR PASSWORD", 8);
                        rememberPassword->Set(35, 45);
                        label *fullscreen = new label("PANTALLA COMPLETA", 8);
                        fullscreen->Set(35, 65);
                        ui_selector *rememberUserSelector = new ui_selector(220, 25);
                        if (configInstance->getBoolValueOf("user")) {
                            rememberUserSelector->addOption("SI");
                            rememberUserSelector->addOption("NO");
                        } else {
                            rememberUserSelector->addOption("NO");
                            rememberUserSelector->addOption("SI");
                        }
                        ui_selector *rememberPassSelector = new ui_selector(220, 45);
                        if (configInstance->getBoolValueOf("pass")) {
                            rememberPassSelector->addOption("SI");
                            rememberPassSelector->addOption("NO");
                        } else {
                            rememberPassSelector->addOption("NO");
                            rememberPassSelector->addOption("SI");
                        }
                        ui_selector *fullScreenSelector = new ui_selector(220, 65);
                        if (configInstance->getBoolValueOf("fullscreen")) {
                            fullScreenSelector->addOption("SI");
                            fullScreenSelector->addOption("NO");
                        } else {
                            fullScreenSelector->addOption("NO");
                            fullScreenSelector->addOption("SI");
                        }
                        settingsContainer->Add(rememberUser);
                        settingsContainer->Add(rememberPassword);
                        settingsContainer->Add(fullscreen);
                        settingsContainer->Add(rememberUserSelector);
                        settingsContainer->Add(rememberPassSelector);
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
    }
}

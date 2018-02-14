#include "event_manager.h"

int event_manager::poll_event() {
    // todo: error handling ?
    return SDL_PollEvent(&lastEvent);
}

void event_manager::process_event() {
    int x = lastEvent.button.x / 2;
    int y = lastEvent.button.y / 2;

    switch (lastEvent.type) {
        case SDL_QUIT:
            gameInstance.cerrar();
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (!userInterface.blocked && userInterface.writing) {
                if (lastEvent.button.button == SDL_BUTTON_LEFT) {
                    if (userInterface.clickOnContainer(x, y)) {
                        auto container = userInterface.getContainerClicked(x, y);

                        if (!container->focused)
                            userInterface.changeContainerFocus(container->index);
                    }

                    if (userInterface.clickOnInputBox(x, y)) {
                        userInterface.changeInputBoxFocus(userInterface.getInputBoxClicked(x, y)->index);
                    } else if (userInterface.clickOnButton(x, y)) {
                        userInterface.changeButtonFocus(userInterface.getButtonClicked(x, y)->index);
                        userInterface.getButtonFocused()->press = 1;
                        userInterface.getContainerClicked(x, y)->buttonPressed = true;
                    } else if (userInterface.clickOnSelector(x, y)) {
                        if (x > (userInterface.getContainerFocused()->x + userInterface.getSelectorClicked(x, y)->x) &&
                            x < (userInterface.getContainerFocused()->x
                                 + userInterface.getSelectorClicked(x, y)->x + 9) &&
                            y > (userInterface.getContainerFocused()->y + userInterface.getSelectorClicked(x, y)->y)
                            && y < (userInterface.getContainerFocused()->y + userInterface.getSelectorClicked(x, y)->y + 14)) {
                            userInterface.getSelectorClicked(x, y)->previous();
                        } else {
                            userInterface.getSelectorClicked(x, y)->next();
                        }
                    }
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (!userInterface.blocked && userInterface.writing && userInterface.getContainerFocused() != nullptr) {
                if (userInterface.getContainerFocused()->buttonPressed) {
                    userInterface.getButtonFocused()->press = 0;
                    userInterface.getContainerFocused()->buttonPressed = false;
                    if (userInterface.clickOnButton(x, y)) {
                        if (userInterface.getButtonClicked(x, y) == userInterface.getButtonFocused())
                            userInterface.execButton(userInterface.getButtonClicked(x, y));
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (!userInterface.blocked) {
                if (lastEvent.key.keysym.sym == SDLK_ESCAPE) {
                    gameInstance.cerrar();
                } else if (lastEvent.key.keysym.sym == SDLK_BACKSPACE) {
                    userInterface.getInputBoxFocused()->remove_last();
                } else if (lastEvent.key.keysym.sym == SDLK_TAB) {
                    // Se focusea al input box siguiente en la std::lista
                    userInterface.changeInputBoxFocus(userInterface.getInputBoxFocused()->index + 1);
                } else if (lastEvent.key.keysym.sym == SDLK_F12) {
                    static ui::container *id = nullptr;
                    if (userInterface.containerExists(id)) {
                        if (id->get_selector(2)->get_selected() == "SI") {
                            config::instance.setValueOf("fullscreen", "1");
                        } else {
                            config::instance.setValueOf("fullscreen", "0");
                        }
                        userInterface.closeContainer(id);
                    } else {
                        // SI NO ESTA CREADO CONSTRUIR MENU OPCIONES
                        auto settingsContainer = new ui::container(362, 180, 300, 100);
                        auto fullscreen = ui::label("PANTALLA COMPLETA", 8);
                        fullscreen.set(35, 65);
                        auto fullScreenSelector = ui::selector(220, 65);
                        if (config::instance.getBoolValueOf("fullscreen")) {
                            fullScreenSelector.add_option("SI");
                            fullScreenSelector.add_option("NO");
                        } else {
                            fullScreenSelector.add_option("NO");
                            fullScreenSelector.add_option("SI");
                        }
                        settingsContainer->add(fullscreen);
                        settingsContainer->add(fullScreenSelector);
                        userInterface.addContainer(settingsContainer);
                        id = settingsContainer;
                        userInterface.blocked = false;
                        userInterface.writing = true;
                    }
                } else if (gameInstance.playing && !userInterface.writing) {
                    if (lastEvent.key.keysym.sym == SDLK_a && playerInstance.x > 0) {
                        playerInstance.moveLeft();
                    } else if (lastEvent.key.keysym.sym == SDLK_s && playerInstance.y < gameInstance.currentMap->height - 1) {
                        playerInstance.moveDown();
                    } else if (lastEvent.key.keysym.sym == SDLK_d && playerInstance.x < gameInstance.currentMap->width - 1) {
                        playerInstance.moveRight();
                    } else if (lastEvent.key.keysym.sym == SDLK_w && playerInstance.y > 0) {
                        playerInstance.moveUp();
                    }
                } else if (userInterface.getInputBoxFocused() != nullptr) {
                    const std::set<SDL_Keycode> allowedSpecialChars = {
                            SDLK_SPACE, SDLK_PERIOD, SDLK_EXCLAIM, SDLK_QUESTION, SDLK_COLON,
                            SDLK_MINUS, SDLK_LEFTPAREN, SDLK_RIGHTPAREN, SDLK_EQUALS
                    };

                    // If the character is allowed, write it
                    if (allowedSpecialChars.find(lastEvent.key.keysym.sym) != allowedSpecialChars.end()) {
                        userInterface.getInputBoxFocused()->write(*SDL_GetKeyName(lastEvent.key.keysym.sym));
                    }

                    if ((lastEvent.key.keysym.sym >= SDLK_0 && lastEvent.key.keysym.sym <= SDLK_9) ||
                        (lastEvent.key.keysym.sym >= SDLK_a && lastEvent.key.keysym.sym <= SDLK_z)) {
                        if (userInterface.writing) {
                            userInterface.getInputBoxFocused()->write(*SDL_GetKeyName(lastEvent.key.keysym.sym));
                        }
                    }
                }
            }

            break;

        default:
            break;
    }
}
#include "event_manager.h"
#include "utils/logger.h"

namespace bure {

event_manager::event_manager() {
  SDL_SetEventFilter([](void *userdata, SDL_Event *e) {
    switch (e->type) {
        case SDL_QUIT:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_KEYDOWN:
          return 1;
          break;
    }

    return 0;
  }, nullptr);
}

void event_manager::pollEvent() {
  SDL_Event lastEvent;

  while (SDL_PollEvent(&lastEvent)) {
    logger::debug("processing event id %d", lastEvent.type);
    for (auto it :
         _eventCallbacks[static_cast<SDL_EventType>(lastEvent.type)]) {
      it.second(lastEvent);
    }
  }
}

callback_handler event_manager::addEventCallback(
    SDL_EventType event_type, std::function<void(SDL_Event e)> fun) {
  logger::debug("adding callback for event id %d", event_type);
  _eventCallbacks[event_type][++lastCallbackHandler] = fun;
  return lastCallbackHandler;
}

void event_manager::removeEventCallback(SDL_EventType event_type,
                                        callback_handler ch) {
  _eventCallbacks[event_type].erase(ch);
}

// void event_manager::process_event(game& game) {
//   int x = lastEvent.button.x / 2;
//   int y = lastEvent.button.y / 2;
//
//   switch (lastEvent.type) {
//     case SDL_QUIT:
//       game.finishGame();
//       break;
//   }
//        case SDL_MOUSEBUTTONDOWN:
//            if (!_ui.blocked && _ui.writing) {
//                if (lastEvent.button.button == SDL_BUTTON_LEFT) {
//                    if (_ui.clickOnContainer(x, y)) {
//                        auto container = _ui.getContainerClicked(x, y);
//
//                        if (!container->focused)
//                            _ui.changeContainerFocus(container->index);
//                    }
//
//                    if (_ui.clickOnInputBox(x, y)) {
//                        _ui.changeInputBoxFocus(_ui.getInputBoxClicked(x,
//                        y)->index);
//                    } else if (_ui.clickOnButton(x, y)) {
//                        _ui.changeButtonFocus(_ui.getButtonClicked(x,
//                        y)->index); _ui.getButtonFocused()->press = 1;
//                        _ui.getContainerClicked(x, y)->buttonPressed = true;
//                    } else if (_ui.clickOnSelector(x, y)) {
//                        if (x > (_ui.getContainerFocused()->x +
//                        _ui.getSelectorClicked(x, y)->x) &&
//                            x < (_ui.getContainerFocused()->x
//                                 + _ui.getSelectorClicked(x, y)->x + 9) &&
//                            y > (_ui.getContainerFocused()->y +
//                            _ui.getSelectorClicked(x, y)->y)
//                            && y < (_ui.getContainerFocused()->y +
//                            _ui.getSelectorClicked(x, y)->y + 14)) {
//                            _ui.getSelectorClicked(x, y)->previous();
//                        } else {
//                            _ui.getSelectorClicked(x, y)->next();
//                        }
//                    }
//                }
//            }
//            break;
//
//        case SDL_MOUSEBUTTONUP:
//            if (!_ui.blocked && _ui.writing && _ui.getContainerFocused() !=
//            nullptr) {
//                if (_ui.getContainerFocused()->buttonPressed) {
//                    _ui.getButtonFocused()->press = 0;
//                    _ui.getContainerFocused()->buttonPressed = false;
//                    if (_ui.clickOnButton(x, y)) {
//                        if (_ui.getButtonClicked(x, y) ==
//                        _ui.getButtonFocused())
//                            _ui.execButton(_ui.getButtonClicked(x, y));
//                    }
//                }
//            }
//            break;
//
//        case SDL_KEYDOWN:
//            if (!_ui.blocked) {
//                if (lastEvent.key.keysym.sym == SDLK_ESCAPE) {
//                    _game.finishGame();
//                } else if (lastEvent.key.keysym.sym == SDLK_BACKSPACE) {
//                    _ui.getInputBoxFocused()->remove_last();
//                } else if (lastEvent.key.keysym.sym == SDLK_TAB) {
//                    // Se focusea al input box siguiente en la std::lista
//                    _ui.changeInputBoxFocus(_ui.getInputBoxFocused()->index +
//                    1);
//                } else if (lastEvent.key.keysym.sym == SDLK_F12) {
//                    static ui::container *id = nullptr;
//                    if (_ui.containerExists(id)) {
//                        if (id->get_selector(2)->get_selected() == "SI") {
//                            config::instance.setValueOf("fullscreen", "1");
//                        } else {
//                            config::instance.setValueOf("fullscreen", "0");
//                        }
//                        _ui.closeContainer(id);
//                    } else {
//                        // todo: this must be built elsewhere
//                        // this does not belong to this class
//                        auto settingsContainer = new ui::container(362, 180,
//                        300, 100); auto fullscreen = ui::label("PANTALLA
//                        COMPLETA", 8); fullscreen.set(35, 65); auto
//                        fullScreenSelector = ui::selector(220, 65); if
//                        (config::instance.getBoolValueOf("fullscreen")) {
//                            fullScreenSelector.add_option("SI");
//                            fullScreenSelector.add_option("NO");
//                        } else {
//                            fullScreenSelector.add_option("NO");
//                            fullScreenSelector.add_option("SI");
//                        }
//                        settingsContainer->add(fullscreen);
//                        settingsContainer->add(fullScreenSelector);
//                        _ui.addContainer(settingsContainer);
//                        id = settingsContainer;
//                        _ui.blocked = false;
//                        _ui.writing = true;
//                    }
//                } else if (_game.playing && !_ui.writing) {
//                    if (lastEvent.key.keysym.sym == SDLK_a &&
//                    _game.getPlayer().x > 0) {
//                        if
//                        (_game.getMap()->comprobarTilePisable(_game.getPlayer().x
//                        - 1, _game.getPlayer().y))
//                        _game.getPlayer().moveLeft();
//                    } else if (lastEvent.key.keysym.sym == SDLK_s &&
//                    _game.getPlayer().y < _game.getMap()->height - 1) {
//                        if
//                        (_game.getMap()->comprobarTilePisable(_game.getPlayer().x,
//                        _game.getPlayer().y - 1))
//                        _game.getPlayer().moveDown();
//                    } else if (lastEvent.key.keysym.sym == SDLK_d &&
//                    _game.getPlayer().x < _game.getMap()->width - 1) {
//                        if
//                        (_game.getMap()->comprobarTilePisable(_game.getPlayer().x,
//                        _game.getPlayer().y + 1))
//                        _game.getPlayer().moveRight();
//                    } else if (lastEvent.key.keysym.sym == SDLK_w &&
//                    _game.getPlayer().y > 0) {
//                        if
//                        (_game.getMap()->comprobarTilePisable(_game.getPlayer().x,
//                        _game.getPlayer().y + 1)) _game.getPlayer().moveUp();
//                    }
//                } else if (_ui.getInputBoxFocused() != nullptr) {
//                    const std::set<SDL_Keycode> allowedSpecialChars = {
//                            SDLK_SPACE, SDLK_PERIOD, SDLK_EXCLAIM,
//                            SDLK_QUESTION, SDLK_COLON, SDLK_MINUS,
//                            SDLK_LEFTPAREN, SDLK_RIGHTPAREN, SDLK_EQUALS
//                    };
//
//                    // If the character is allowed, write it
//                    if (allowedSpecialChars.find(lastEvent.key.keysym.sym) !=
//                    allowedSpecialChars.end()) {
//                        _ui.getInputBoxFocused()->write(*SDL_GetKeyName(lastEvent.key.keysym.sym));
//                    }
//
//                    if ((lastEvent.key.keysym.sym >= SDLK_0 &&
//                    lastEvent.key.keysym.sym <= SDLK_9) ||
//                        (lastEvent.key.keysym.sym >= SDLK_a &&
//                        lastEvent.key.keysym.sym <= SDLK_z)) { if
//                        (_ui.writing) {
//                            _ui.getInputBoxFocused()->write(*SDL_GetKeyName(lastEvent.key.keysym.sym));
//                        }
//                    }
//                }
//            }
//
//            break;
//
//        default:
//            break;
//    }
//}

// template <class T>
// void event_manager::addEventCallback(std::function<void(T)> callback) {
//   auto id = T::EVENT_ID;
//   _eventCallbacks[id].push_back(callback);
// }

}  // namespace bure

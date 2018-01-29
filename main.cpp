#include "SDL.h"
#include "Graficos.h"
#include "Interface.h"
#include "Error.h"
#include "Jugador.h"
#include "Mapa.h"
#include "Network.h"
#include "Juego.h"
#include "Config.h"

// Los tres hilos
SDL_Thread *hiloEnviar, *hiloRecibir;
SDL_Event evento;

// Objetos principales
Graficos *pantalla;
Network *net;
Interface *interfaz;
Jugador *player;
Juego *game;
Config *cfg;

// Path a las imagenes y demas
#ifdef __LOCAL_RESOURCES__
std::string path = "";
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

int main()  {
	atexit(SDL_Quit);

	// Iniciar SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		Error::Log(SDL_GetError(), 3);
	}

	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(400, 100);
	SDL_WM_SetCaption("Bure Online 0.2.0", NULL);

	// Iniciamos objetoss
	cfg = new Config;
	pantalla = new Graficos;
	interfaz = new Interface;
	player = new Jugador;
	net = new Network;
	game = new Juego;

	interfaz->changeIBFocus(0);

	// Creamos el hilo que envia, y el que recibe
	// Tambien el que recibe del servidor y lee la entrada del teclado y el raton
	hiloEnviar = SDL_CreateThread(enviar, NULL);
	hiloRecibir = SDL_CreateThread(recibir, NULL);

	while(game->abierto) {
		if(game->princp) {
			game->princp_parado = false;
			if(game->jugando) {
				pantalla->Limpiar();
				pantalla->Dibujar(game->mapaActual, false);
				pantalla->DibujarJugadores();
				pantalla->Dibujar(game->mapaActual, true);
				game->MensajesLoop();
				game->nextAnimationFrame();
			} else {
			    pantalla->RenderBg();
			}
			pantalla->Dibujar(interfaz);
			pantalla->Volcar();

			while(SDL_PollEvent(&evento)) manageEvent();
			if(net->intentoConexion > 0) net->intentoConexion++;
			if(net->intentoConexion > 150) net->traducirPaquete("1_0_NOTSERVER");

			SDL_Delay(10);
		} else {
			game->princp_parado = true;
			SDL_Delay(100);
		}
	}

	SDL_KillThread(hiloEnviar);
	SDL_KillThread(hiloRecibir);

	delete pantalla;
	delete net;
	delete interfaz;
	delete player;
	delete game;

	SDL_Quit();

	return 0;
}

///////////////////////////////////////////////////
///////////////// HILO DE EVENTOS /////////////////
///////////////////////////////////////////////////

void manageEvent() {
    int x = evento.button.x;
	int y = evento.button.y;

    switch(evento.type) {
        case SDL_QUIT:
        	if(game->logged) net->sendPacket("0_0_NOTHING");
        	SDL_Delay(250);
            game->cerrar();
            break;

        case SDL_MOUSEBUTTONDOWN:
            if(!interfaz->bloqueado && interfaz->writing) {
		        if(evento.button.button == SDL_BUTTON_LEFT ) {
                    if(interfaz->clickOnContainer(x, y)) {
                        if(interfaz->getContainerClicked(x, y)->getFocus() == 0) interfaz->changeContainerFocus(interfaz->getContainerClicked(x, y)->index);
                    }

                    if(interfaz->clickOnIB(x, y)) {
                        interfaz->changeIBFocus(interfaz->getInputBoxClicked(x, y)->index);
                    } else if(interfaz->clickOnButton(x, y)) {
                        interfaz->changeButtonFocus(interfaz->getButtonClicked(x, y)->index);
                        interfaz->getButtonFocused()->press = 1;
                        interfaz->getContainerClicked(x, y)->boton_pulsado = true;
                    } else if(interfaz->clickOnSelector(x, y)) {
                    	if( x > (interfaz->getContainerFocused()->x + interfaz->getSelectorClicked(x, y)->x) && x < (interfaz->getContainerFocused()->x
						+ interfaz->getSelectorClicked(x, y)->x + 9) && y > (interfaz->getContainerFocused()->y + interfaz->getSelectorClicked(x, y)->y)
						&& y < (interfaz->getContainerFocused()->y + interfaz->getSelectorClicked(x, y)->y + 14)) {
                    		interfaz->getSelectorClicked(x, y)->previous();
                    	} else {
                    		interfaz->getSelectorClicked(x, y)->next();
                    	}
                    }
		        }
	        }
            break;

        case SDL_MOUSEBUTTONUP:
	        if(!interfaz->bloqueado && interfaz->writing && interfaz->getContainerFocused() != NULL) {
	            if(interfaz->getContainerFocused()->boton_pulsado) {
                    interfaz->getButtonFocused()->press = 0;
                    interfaz->getContainerFocused()->boton_pulsado = false;
                    if(interfaz->clickOnButton(x, y)) {
                        if(interfaz->getButtonClicked(x, y) == interfaz->getButtonFocused()) interfaz->ejecutarBoton(interfaz->getButtonClicked(x, y));
                    }
                }
	        }
            break;

        case SDL_KEYDOWN:
            if(!interfaz->bloqueado) {
            	if(evento.key.keysym.sym == SDLK_ESCAPE) {
            		if(game->logged) net->sendPacket("0_0_NOTHING");
            		SDL_Delay(250);
			        game->cerrar();
	        } else if(evento.key.keysym.sym == SDLK_BACKSPACE) {
		       interfaz->getInputBoxFocused()->Borrar();
	        } else if(evento.key.keysym.sym == SDLK_RETURN) {
	        	if(interfaz->writing) {
			        if(interfaz->getInputBoxFocused() != NULL && interfaz->getInputBoxFocused()->function != NULL) {
				        interfaz->ejecutarBoton(interfaz->getInputBoxFocused());
			        }
		        } else if(game->jugando) {
			        // MOSTRAR INPUT BOX PARA ENVIAR MENSAJE
			        SDL_EnableKeyRepeat(400, 100);

			        Container *con = new Container(0, 0, 1024, 768);
			        con->SetInvisible();

			        InputBox *user = new InputBox("Mensaje:", "", 60);
			        user->Set(80, 748, 400);
			        user->function = &Interface::e_enviarMensaje;
			        user->escapeable(true);
			        con->Add(user);

			        interfaz->addContainer(con);
			        interfaz->writing = true;
		        }
		        } else if(evento.key.keysym.sym == SDLK_TAB) {
			        // Se focusea al input box siguiente en la std::lista
			        interfaz->changeIBFocus(interfaz->getInputBoxFocused()->index+1);
		        } else if(evento.key.keysym.sym == SDLK_F12) {
		        	static Container *id = NULL;
		        	if(interfaz->containerExists(id)) {
		        		// SI ESTA CREADO GUARDAR OPCIONES Y SALIR
	        			if(!id->getSelector(0)->getSelected().compare("SI")) {
	        				if(!cfg->getValueOf("user").compare("0")) {
							cfg->setValueOf("user",  "User");
						}
						} else {
							cfg->setValueOf("user",  "0");
						}
						if(!id->getSelector(1)->getSelected().compare("SI")) {
							if(!cfg->getValueOf("pass").compare("0")) {
								cfg->setValueOf("pass",  "Pass");
							}
						} else {
							cfg->setValueOf("pass",  "0");
						}
						if(!id->getSelector(2)->getSelected().compare("SI")) {
							cfg->setValueOf("fullscreen",  "1");
						} else {
							cfg->setValueOf("fullscreen",  "0");
						}
		        		interfaz->closeContainer(id);
		        	} else {
		        		// SI NO ESTA CREADO CONSTRUIR MENU OPCIONES
		        		Container *con = new Container(362, 180, 300, 100);
	        			Label *guardaruser = new Label("RECORDAR USUARIO", 8);
						guardaruser->Set(35, 25);
					Label *guardarpass = new Label("RECORDAR PASSWORD", 8);
						guardarpass->Set(35, 45);
					Label *pcompleta = new Label("PANTALLA COMPLETA", 8);
						pcompleta->Set(35, 65);
		        		Selector *seled = new Selector(220, 25);
		        			if(cfg->getBoolValueOf("user")) {
							seled->addOption("SI");
							seled->addOption("NO");
						} else {
							seled->addOption("NO");
							seled->addOption("SI");
						}
					Selector *seled2 = new Selector(220, 45);
						if(cfg->getBoolValueOf("pass")) {
							seled2->addOption("SI");
							seled2->addOption("NO");
						} else {
							seled2->addOption("NO");
							seled2->addOption("SI");
						}
					Selector *seled3 = new Selector(220, 65);
						if(cfg->getBoolValueOf("fullscreen")) {
							seled3->addOption("SI");
							seled3->addOption("NO");
						} else {
							seled3->addOption("NO");
							seled3->addOption("SI");
						}
					con->Add(guardaruser);
					con->Add(guardarpass);
					con->Add(pcompleta);
					con->Add(seled);
					con->Add(seled2);
					con->Add(seled3);
					interfaz->addContainer(con);
					id = con;
					interfaz->bloqueado = false;
					interfaz->writing = true;
		        	}
		        } else if(game->jugando && !interfaz->writing) {
				        if((char)evento.key.keysym.unicode == 'a' || (char)evento.key.keysym.unicode == 'A') {
					        if(player->x > 0) {
						        player->moveLeft();
					        }
				        } else if((char)evento.key.keysym.unicode == 's' || (char)evento.key.keysym.unicode == 'S') {
					        if(player->y < game->mapaActual->mapa_alto-1) {
						        player->moveDown();
					        }
				        } else if((char)evento.key.keysym.unicode == 'd' || (char)evento.key.keysym.unicode == 'D') {
					        if(player->x < game->mapaActual->mapa_ancho-1) {
						        player->moveRight();
					        }
				        } else if((char)evento.key.keysym.unicode == 'w' || (char)evento.key.keysym.unicode == 'W') {
					        if(player->y > 0) {
						        player->moveUp();
					        }
				        }
	   		 } else if(interfaz->getInputBoxFocused() != NULL) {
			    	if(interfaz->getInputBoxFocused()->escapeable()) {
						if(evento.key.keysym.unicode == (Uint16)' ' || evento.key.keysym.unicode == (Uint16)',' ||
						evento.key.keysym.unicode == (Uint16)'.' || evento.key.keysym.unicode == (Uint16)'!' ||
						evento.key.keysym.unicode == (Uint16)'�' || evento.key.keysym.unicode == (Uint16)'?' ||
						evento.key.keysym.unicode == (Uint16)'�' || evento.key.keysym.unicode == (Uint16)':' ||
						evento.key.keysym.unicode == (Uint16)'-' || evento.key.keysym.unicode == (Uint16)'('
						|| evento.key.keysym.unicode == (Uint16)')' || evento.key.keysym.unicode == (Uint16)'=') {
							interfaz->getInputBoxFocused()->Escribir((char)evento.key.keysym.unicode);
						}
			   		}

			   		if((evento.key.keysym.unicode & 0xFF80) == 0 && ((evento.key.keysym.unicode >= 48 &&
		        	evento.key.keysym.unicode <= 57) || (evento.key.keysym.unicode >= 65 && evento.key.keysym.unicode
		        	<= 90) || (evento.key.keysym.unicode >= 97 && evento.key.keysym.unicode <= 122))) {
			   			if(interfaz->writing) {
			   				interfaz->getInputBoxFocused()->Escribir((char)evento.key.keysym.unicode);
		   				}
			   		}
			   	}
			}
            break;
    }
}

#include "game.h"

extern player *playerInstance;

game::game()
{
    currentMap = nullptr;
    abierto = true;
    playing = false;
    logged = false;
}

void game::cerrar()
{
    abierto = false;
    playing = false;
}

void game::cambiarMapa(std::string archivo)
{
	delete currentMap;
    // todo: fix base path
	currentMap = new game_map("/home/carlosrdrz/dev/bure", archivo);
}

void game::nextAnimationFrame()
{
	playerInstance->nextWalkingFrame();
}

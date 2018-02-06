#include <string>
#include "label.h"

ui::label::label(std::string title, int tam)
{
	texto = title;
    tamano = tam;
}

void ui::label::Set(int x, int y)
{
	this->x = x;
	this->y = y;
}

void ui::label::cambiarTexto(std::string que)
{
	this->texto = que;
}

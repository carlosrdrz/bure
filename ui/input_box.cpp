#include "input_box.h"
#include "../error.h"

#include <string>

ui::input_box::input_box()
{
	this->focus = 0; 
	this->index = -1;
	this->secreto = 0;
	this->maxchar = 12;
	this->parametro = 0;
	this->function = NULL;
	this->charsplus = false;
}

ui::input_box::input_box(std::string title, std::string prf, int maxchars)
{
	this->titulo = title;
	this->texto = prf;
	this->focus = 0;
	this->index = -1;
	this->secreto = 0;
	this->maxchar = maxchars;
	this->parametro = 0;
	this->function = NULL;
	this->charsplus = false;
}

void ui::input_box::Set(int x, int y, int anc)
{
	this->x = x;
	this->y = y;
	this->width = anc;
	this->height = 14;
}

void ui::input_box::Borrar()
{
	if((int)texto.size() != 0) {
		texto = texto.substr(0, texto.length()-1);
	}
}

void ui::input_box::write(char que)
{
	if((int)texto.size() >= maxchar) return;
	texto.push_back(que);
}

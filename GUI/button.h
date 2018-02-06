#pragma once

#include "widget.h"
#include "../user_interface.h"
#include <string>

class user_interface;

typedef void (user_interface::*InterfaceFunc)(int param);

class button : public widget {
public:
	std::string titulo;
	InterfaceFunc function;
	int press;	
	int parametro;

	button() { this->press = 0; this->parametro = 0; }
	button(std::string title) {
		this->titulo = title;
		this->press = 0;
		this->parametro = 0;
	}
	~button() {}

	const char *getTitulo() { return titulo.c_str(); }

	void Set(int x, int y, int anc, int larg) {
		this->x = x;
		this->y = y;
		this->width = anc;
		this->height = larg;
	}
};

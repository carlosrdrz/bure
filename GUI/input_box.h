#pragma once

#include "widget.h"
#include "../user_interface.h"
#include <string>

class user_interface;

typedef void (user_interface::*InterfaceFunc)(int param);

class input_box : public widget {
public:
	InterfaceFunc function;
	std::string texto;
	std::string titulo;
	int parametro;
	int secreto;
	
	input_box();
	input_box(std::string title, std::string prf, int maxchars = 12);
	~input_box() {}

	void Set(int x, int y, int anc);
	
	void write(char que);
	void Borrar();

	void EsSecreto() { this->secreto = 1; }
	bool escapeable() { return charsplus; }
	void escapeable(bool que) { charsplus = que; }
	
	void Focus() { focus = 1; }
	void noFocus() { focus = 0; }
	int getFocus() { return focus; }
	
	const char *getTexto() { return texto.c_str(); }
	const char *getTitulo() { return titulo.c_str(); }
	int getCaracteres() { return (int)texto.size(); }
	
private:
	int focus;
	int maxchar;
	bool charsplus;
};

#pragma once

#include "widget.h"
#include <string>
#include <functional>

class user_interface;

namespace ui {
	class button : public widget {
	public:
		using callback = std::function<void(int)>;

		std::string titulo;
		callback function;
		int press;
		int parametro;

		button() {
			this->press = 0;
			this->parametro = 0;
		}

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
};
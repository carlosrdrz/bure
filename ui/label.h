#pragma once

#include "widget.h"
#include <string>

namespace ui {
	class label : public widget {
		public:
			label(std::string title, int tam = 16);
			~label() {}

			void Set(int x, int y);
			const char *getText() { return texto.c_str(); }
			void cambiarTexto(std::string que);
			int getTamano() { return tamano; }
		private:
			std::string texto;
			int tamano;
	};
};

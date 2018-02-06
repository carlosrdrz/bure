#pragma once

#include "SDL_image.h"
#include "widget.h"
#include <string>

namespace ui {
	class image : public widget {
	public:
		SDL_Surface *img;
		int selector;
		std::string currentpath;
		std::string inicio;
		std::string final;

		image() {
			img = nullptr;
			dinamic = false;
		}

		image(std::string path) {
			img = IMG_Load(path.c_str());
			dinamic = false;
		}

		~image() { SDL_FreeSurface(img); }

		void set(int x, int y) {
			this->x = x;
			this->y = y;
		}

		void setDinamic() { dinamic = true; }

		void setDinamicSelector(int sel, std::string init, std::string fin) {
			selector = sel;
			inicio = init;
			final = fin;
		}

		bool isDinamic() { return dinamic; }

	private:
		bool dinamic;
	};
};
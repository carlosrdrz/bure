#pragma once

#include "widget.h"

#include <list>
#include <string>

namespace ui {
	class selector : public widget {
	public:
		selector(int x, int y);

		~selector();

		void addOption(std::string toadd);

		std::string getSelected();

		void next();

		void previous();

		std::list<std::string>::iterator opcionesit;

	private:
		std::list<std::string> opciones;
	};
};
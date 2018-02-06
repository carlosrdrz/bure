#pragma once

#include "widget.h"

#include <list>
#include <string>

class ui_selector : public widget {
	public:
		ui_selector(int x, int y);
		~ui_selector();
		
		void addOption(std::string toadd);
		std::string getSelected();
		
		void next();
		void previous();
		
		std::list<std::string>::iterator opcionesit;
				
	private:
		std::list<std::string> opciones;
};

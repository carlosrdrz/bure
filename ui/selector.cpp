#include "selector.h"

#include <list>
#include <string>

ui::selector::selector(int x, int y)
{
	options.push_back("DEFAULT");
	options_it = options.begin();
	this->x = x;
	this->y = y;
	this->height = 14;
	this->width = 0;
}

ui::selector::~selector()
{
	options.clear();
}

void ui::selector::add_option(std::string value)
{
	if(options.front().compare("DEFAULT") == 0) {
		options.clear();
	}
	options.push_back(value);
}

std::string ui::selector::get_selected()
{
	return (*options_it);
}

void ui::selector::next()
{
	if(options.size() > 1) {
		if((*options_it).compare(options.back()) == 0) options_it = options.begin();
		else options_it++;
	}
}

void ui::selector::previous()
{
	if(options.size() > 1) {
		if(options_it == options.begin()) {
			options_it = options.end();
			options_it--;
		} else {
			options_it--;
		}
	}
}

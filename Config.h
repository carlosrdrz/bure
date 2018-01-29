#ifndef __H_CONFIG__
#define __H_CONFIG__

#include "libxml/tree.h"
#include <string>

class Config {
	public:
		Config();
		~Config();

		std::string getValueOf(std::string of);
		bool getBoolValueOf(std::string of);
		void setValueOf(std::string of, std::string on);

	private:
		xmlDocPtr doc;
		xmlNodePtr node;
};
#endif

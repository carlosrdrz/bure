#include "config.h"
#include "error.h"

extern std::string path;

config::config() {
	if ((doc = xmlParseFile((path+"data/config.xml").c_str())) == nullptr) {
		Error::Log("No se pudo cargar el archivo config.xml", 3);
	}

	node = xmlDocGetRootElement(doc);
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) {
		node = node->next;
	}
}

config::~config() {
	xmlFreeNode(node);
	xmlFreeDoc(doc);
}

std::string config::getValueOf(std::string of) {
	std::string toreturn;
	xmlChar *temp;

	while(node != nullptr) {
		if(!of.compare((char*)node->name)) {
			temp = xmlGetProp(node, (xmlChar*)"value");
			toreturn =  (char*)temp;
			free(temp);
			break;
		} else {
			// Avanzar
			node = node->next;
			while(xmlNodeIsText(node)) {
				node = node->next;
			}
		}
	}

	node = xmlDocGetRootElement(doc);
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) {
		node = node->next;
	}

	return toreturn;
}

bool config::getBoolValueOf(std::string of) {
	bool toreturn = true;
	std::string cero = "0";
	xmlChar *temp;

	while(node != nullptr) {
		if(!of.compare((char*)node->name)) {
			xmlChar *temp = xmlGetProp(node, (xmlChar*)"value");
			if(!cero.compare((char*)temp)) {
				toreturn = false;
			}
			break;
		} else {
			// Avanzar
			node = node->next;
			while(xmlNodeIsText(node)) {
				node = node->next;
			}
		}
	}

	node = xmlDocGetRootElement(doc);
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) {
		node = node->next;
	}

	return toreturn;
}

void config::setValueOf(std::string of, std::string on) {
	while(node != nullptr) {
		if(!of.compare((char*)node->name)) {
			// Cambiar
			xmlSetProp(node, (xmlChar*)"value", (xmlChar*)on.c_str());
			xmlSaveFileEnc((path+"data/config.xml").c_str(), doc, "UTF-8");
			break;
		} else {
			// Avanzar
			node = node->next;
			while(xmlNodeIsText(node)) {
				node = node->next;
			}
		}
	}

	node = xmlDocGetRootElement(doc);
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) {
		node = node->next;
	}
}

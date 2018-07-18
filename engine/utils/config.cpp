#include "config.h"

void config::readFile(std::string path) {
  if (path.empty()) {
    return;
  }

  xmlpp::DomParser parser;
  parser.parse_file(path + "data/config.xml");

  if (!parser) {
    Error::Log("Could not load config file", 3);
    return;
  }

  // Get the root element
  const auto rootNode = parser.get_document()->get_root_node();

  for (const auto& child : rootNode->get_children()) {
    auto node = static_cast<xmlpp::Element*>(child);
    std::string name = node->get_name();
    std::string value = node->get_attribute_value("value");
    configs[name] = value;
  }
}

bool config::getBoolValueOf(std::string of) { return ("1" == configs[of]); }

void config::setValueOf(std::string of, std::string on) { configs[of] = on; }

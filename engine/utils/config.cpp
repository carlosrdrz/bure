#include "config.h"

#include "libxml++/libxml++.h"
#include "logger.h"

namespace bure {

void config::readFile(std::string path) {
  if (path.empty()) {
    return;
  }

  auto configPath = path + "resources/config.xml";
  xmlpp::DomParser parser;
  parser.parse_file(configPath);

  if (!parser) {
    logger::error("could not load config file %s", configPath.c_str());
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

float config::getFloatValueOf(std::string of, float defaultValue) {
  try {
    auto configValue = configs.at(of);
    return stof(configValue, nullptr);
  } catch (std::out_of_range& e) {
    return defaultValue;
  }
}

void config::setValueOf(std::string of, std::string on) { configs[of] = on; }

}  // namespace bure

#include "game_map.h"
#include <libxml++/libxml++.h>
#include "utils/logger.h"

namespace bure {

game_map::game_map(std::string basePath, std::string archivo) {
  mapa = nullptr;
  elementos = nullptr;
  pisable = nullptr;

  xmlpp::DomParser parser;
  parser.parse_file(basePath + "/maps/" + archivo);

  if (!parser) logger::log("Could not load map " + archivo, 2);

  // Get the root element
  const auto rootNode = parser.get_document()->get_root_node();
  const auto widthAttr = rootNode->get_attribute_value("width");
  const auto heightAttr = rootNode->get_attribute_value("height");

  width = atoi(widthAttr.c_str());
  height = atoi(heightAttr.c_str());

  mapa = new int[height * width];
  elementos = new int[height * width];
  pisable = new bool[height * width];

  auto layerNode = rootNode->get_first_child("layer");
  auto dataNode = layerNode->get_first_child("data");

  int num = 0;
  for (const auto& child : dataNode->get_children("tile")) {
    auto node = static_cast<xmlpp::Element*>(child);
    std::string value = node->get_attribute_value("gid");
    mapa[num++] = atoi(value.c_str());
  }

  do {
    layerNode = layerNode->get_next_sibling();
  } while (layerNode->get_name() != "layer");

  dataNode = layerNode->get_first_child("data");

  num = 0;
  for (const auto& child : dataNode->get_children("tile")) {
    auto node = static_cast<xmlpp::Element*>(child);
    std::string value = node->get_attribute_value("gid");
    elementos[num++] = atoi(value.c_str());
  }

  do {
    layerNode = layerNode->get_next_sibling();
  } while (layerNode->get_name() != "layer");

  dataNode = layerNode->get_first_child("data");

  num = 0;
  for (const auto& child : dataNode->get_children("tile")) {
    auto node = static_cast<xmlpp::Element*>(child);
    std::string value = node->get_attribute_value("gid");
    pisable[num++] = (atoi(value.c_str()) == 0);
  }
}

game_map::~game_map() {
  delete[] mapa;
  delete[] elementos;
  delete[] pisable;
}

bool game_map::comprobarTilePisable(int x, int y) {
  return pisable[x + (y * width)];
}

}  // namespace bure

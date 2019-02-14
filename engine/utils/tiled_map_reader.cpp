#include "tiled_map_reader.h"
#include "utils/logger.h"

#include <libxml++/libxml++.h>
#include <fstream>

std::unique_ptr<bure::game_map> tiled_map_reader::read(std::string file) {
  xmlpp::DomParser parser;
  parser.parse_file(file);

  if (!parser) bure::logger::error("could not load map %s", file.c_str());

  // Get the root element
  const auto rootNode = parser.get_document()->get_root_node();
  const auto widthAttr = rootNode->get_attribute_value("width");
  const auto heightAttr = rootNode->get_attribute_value("height");
  const auto tileWidthAttr = rootNode->get_attribute_value("tilewidth");
  const auto tileHeightAttr = rootNode->get_attribute_value("tileheight");

  auto width = std::stoi(widthAttr);
  auto height = std::stoi(heightAttr);
  auto tileWidth = std::stoi(tileWidthAttr);
  auto tileHeight = std::stoi(tileHeightAttr);

  // the result map
  auto map =
      std::make_unique<bure::game_map>(width, height, tileWidth, tileHeight);

  // Get tilesets
  int tilesetId = 0;
  auto tilesetNodes = rootNode->get_children("tileset");
  for (const auto& tilesetNode : tilesetNodes) {
    bure::tileset ts;

    auto node = static_cast<xmlpp::Element*>(tilesetNode);
    ts.name = node->get_attribute_value("name");
    ts.firstGid = std::stoi(node->get_attribute_value("firstgid"));

    auto imageChild = tilesetNode->get_first_child("image");
    auto imageNode = static_cast<xmlpp::Element*>(imageChild);
    ts.file = imageNode->get_attribute_value("source");

    auto gridChild = tilesetNode->get_first_child("grid");
    auto gridNode = static_cast<xmlpp::Element*>(gridChild);
    ts.gridWidth = std::stoi(gridNode->get_attribute_value("width"));
    ts.gridHeight = std::stoi(gridNode->get_attribute_value("height"));

    ts.id = tilesetId++;
    map->addTileset(ts);
  }

  // Get layers
  auto layerNodes = rootNode->get_children("layer");
  for (const auto& layerNode : layerNodes) {
    bure::layer l;

    auto node = static_cast<xmlpp::Element*>(layerNode);
    auto data = layerNode->get_first_child("data");
    l.id = std::stoi(node->get_attribute_value("id"));
    l.visible = !(node->get_attribute_value("visible") == "0");
    l.name = node->get_attribute_value("name");
    l.data = new int[height * width];

    int num = 0;
    for (const auto& child : data->get_children("tile")) {
      auto tileNode = static_cast<xmlpp::Element*>(child);
      auto value = tileNode->get_attribute_value("gid");
      if (value.empty())
        l.data[num++] = 0;
      else
        l.data[num++] = std::stoi(value);
    }

    map->addLayer(l);
  }

  return map;
}

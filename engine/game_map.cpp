#include "game_map.h"
#include "utils/logger.h"

#include <libxml++/libxml++.h>
#include <fstream>
#include <iostream>

namespace bure {

game_map::game_map(std::string basePath, std::string file) {
  xmlpp::DomParser parser;
  parser.parse_file(basePath + "/resources/maps/" + file);

  if (!parser) logger::error("could not load map %s", file.c_str());

  // Get the root element
  const auto rootNode = parser.get_document()->get_root_node();
  const auto widthAttr = rootNode->get_attribute_value("width");
  const auto heightAttr = rootNode->get_attribute_value("height");
  const auto tileWidthAttr = rootNode->get_attribute_value("tilewidth");
  const auto tileHeightAttr = rootNode->get_attribute_value("tileheight");

  _width = std::stoi(widthAttr);
  _height = std::stoi(heightAttr);
  _tileWidth = std::stoi(tileWidthAttr);
  _tileHeight = std::stoi(tileHeightAttr);

  // Get tilesets
  int tilesetId = 0;
  auto tilesetNodes = rootNode->get_children("tileset");
  for (const auto& tilesetNode : tilesetNodes) {
    tileset ts;

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
    _tilesets.push_back(ts);
  }

  // Get layers
  auto layerNodes = rootNode->get_children("layer");
  for (const auto& layerNode : layerNodes) {
    layer l;

    auto node = static_cast<xmlpp::Element*>(layerNode);
    auto data = layerNode->get_first_child("data");
    l.id = std::stoi(node->get_attribute_value("id"));
    l.visible = !(node->get_attribute_value("visible") == "0");
    l.name = node->get_attribute_value("name");
    l.data = new int[_height * _width];

    int num = 0;
    for (const auto& child : data->get_children("tile")) {
      auto tileNode = static_cast<xmlpp::Element*>(child);
      auto value = tileNode->get_attribute_value("gid");
      if (value.empty()) l.data[num++] = 0  ;
      else l.data[num++] = std::stoi(value);
    }

    _layers.push_back(l);
  }
}

game_map::~game_map() {
  for (auto& layer : _layers) {
    delete layer.data;
  }
}

void game_map::setScale(int scale) { _scale = scale; }
int game_map::getScale() { return _scale; }

int game_map::getWidth() { return _width; }
int game_map::getHeight() { return _height; }
int game_map::getTileWidth() { return _tileWidth * _scale; }
int game_map::getTileHeight() { return _tileHeight * _scale; }

layer game_map::getLayer(int layerId) {
  return _layers.at(layerId);
}

tile game_map::getTileData(int tileGid) {
  if (_tiles.count(tileGid) > 0) return _tiles[tileGid];

  tile t;
  tileset ts;
  for (auto tst : _tilesets) {
    if (tileGid >= tst.firstGid) {
      ts = tst;
    }
  }

  t.file = ts.file;
  t.srcX = 0;
  t.srcY = 0;
  t.srcWidth = _tileWidth;
  t.srcHeight = _tileHeight;

  auto tileTsId = tileGid - ts.firstGid + 1;
  while (tileTsId > ts.gridWidth) {
    tileTsId -= ts.gridWidth;
    t.srcY += _tileHeight;
  }

  t.srcX = (tileTsId - 1) * _tileWidth;

  _tiles[tileGid] = t;
  return t;
}

screen_coords game_map::mapToScreen(map_coords m) {
  return {
    m.x * getTileWidth(),
    m.y * getTileHeight()
  };
}

map_coords game_map::screenToMap(screen_coords s) {
  return {
    s.x / getTileWidth(),
    s.y / getTileHeight()
  };
}

}  // namespace bure

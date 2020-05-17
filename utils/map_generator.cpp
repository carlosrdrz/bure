#include "map_generator.h"

#include <libxml++/libxml++.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <random>

#include "utils/logger.h"
#include "utils/pathfinding.h"

#define MAX_ITERATIONS 1000
#define PADDING 10
#define DEFAULT_TILE 5
#define CUBE_DATA_LENGTH 9
#define TILE_WIDTH 32
#define TILE_HEIGHT 32
#define MAX_RATIO 2.22
#define MAX_SPACE_IN_SECTION 0.6

map_generator::map_generator(const std::string &file) {
  xmlpp::DomParser parser;
  parser.parse_file(file);

  if (!parser) {
    bure::logger::error("could not load map enricher file %s", file.c_str());
  }

  // Get the root element
  const auto *rootNode = parser.get_document()->get_root_node();
  const auto *tilesNode = rootNode->get_first_child("tiles");
  const auto *mapsNode = rootNode->get_first_child("maps");

  // Read tiles
  auto tileNodes = tilesNode->get_children("tile");
  for (auto &tileNode : tileNodes) {
    const auto *node = dynamic_cast<const xmlpp::Element *>(tileNode);
    if (node == nullptr) {
      continue;
    }

    const auto *nameAttr = node->get_attribute("name");
    const auto *idAttr = node->get_attribute("id");
    if (nameAttr != nullptr && idAttr != nullptr) {
      auto name = nameAttr->get_value();
      auto id = idAttr->get_value();
      _tiles[name] = std::stoi(std::string(id));
    }
  }

  // Read maps
  auto mapNodes = mapsNode->get_children("map");
  for (const auto &mapNode : mapNodes) {
    const auto *node = dynamic_cast<const xmlpp::Element *>(mapNode);
    if (node == nullptr) {
      continue;
    }

    const auto *dataAttr = node->get_attribute("data");
    const auto *tileNameAttr = node->get_attribute("tileName");
    if (dataAttr != nullptr && tileNameAttr != nullptr) {
      auto data = dataAttr->get_value();
      auto tileName = tileNameAttr->get_value();
      auto tileId = _tiles[tileName];

      // TODO: what if tiles not contain tileName
      if (data.length() != CUBE_DATA_LENGTH) {
        bure::logger::error("error reading enricher data %s tileName %d",
                            data.c_str(), tileId);
        continue;
      }

      // clang-format off
      // Ignoring format and linter here for readability
      // We are transforming the string data read from the cube data
      // into actual numbers that we can use in a cube
      // TODO: Does this even work with other file encodings, like unicode? Probably not
      struct cube readCube {
        data[0] - '0', // NOLINT
        data[1] - '0', // NOLINT
        data[2] - '0', // NOLINT
        data[3] - '0', // NOLINT
        data[4] - '0', // NOLINT
        data[5] - '0', // NOLINT
        data[6] - '0', // NOLINT
        data[7] - '0', // NOLINT
        data[8] - '0', // NOLINT
      };
      // clang-format on

      _cubeTiles.push_back({readCube, tileId});
    }
  }
}

std::unique_ptr<bure::game_map> map_generator::generate(int w, int h) {
  auto width = w + PADDING * 2;
  auto height = h + PADDING * 2;

  auto map =
      std::make_unique<bure::game_map>(width, height, TILE_WIDTH, TILE_HEIGHT);

  auto *mainLayerData = new int[height * width];
  auto *skeletonLayerData = new int[height * width];

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      mainLayerData[i * width + j] = DEFAULT_TILE;
      skeletonLayerData[i * width + j] = 0;
    }
  }

  auto s = section({{PADDING, PADDING}, {width - PADDING, height - PADDING}});
  auto *tree = sectionize(s, _splites);
  auto rooms = generateRooms(tree);

  drawRooms(rooms, skeletonLayerData, width);
  drawPaths(tree, skeletonLayerData, _splites, width);
  drawTiles(skeletonLayerData, mainLayerData, width, height, _cubeTiles);

  bure::layer mainLayer = {0, "terrain", mainLayerData, true};
  map->addLayer(mainLayer);

  bure::layer skeletonLayer = {1, "pisable", skeletonLayerData, false};
  map->addLayer(skeletonLayer);

  // clang-format off
  bure::tileset ts = {
      0,                                      // NOLINT: ID
      "terrain_tileset",                      // NOLINT: Name
      "../sprites/tileset_dungeon_mod.png",   // NOLINT: File
      1,                                      // NOLINT: First GID
      8,                                      // NOLINT: Tileset width
      32                                      // NOLINT: Tileset height
  };
  // clang-format on

  map->addTileset(ts);

  return map;
}

node *map_generator::sectionize(section s, int splites) {
  section section_left;
  section section_right;
  auto iter = 0;

  if (splites == 0) {
    return new node({s, s, nullptr, nullptr, 0});
  }

  do {
    if (iter++ >= MAX_ITERATIONS) {
      bure::logger::error("max iterations reached in sectionize()");
      exit(1);
    }

    if (random(0, 1) == 0) {
      auto pivot = random(s.p1.x + 1, s.p2.x - 1);  // Split horizontally
      section_left = section({s.p1, {pivot, s.p2.y}});
      section_right = section({{(pivot + 1), s.p1.y}, s.p2});
    } else {
      auto pivot = random(s.p1.y + 1, s.p2.y - 1);  // Split vertically
      section_left = section({s.p1, {s.p2.x, pivot}});
      section_right = section({{s.p1.x, (pivot + 1)}, s.p2});
    }
  } while (!validSection(section_left) || !validSection(section_right));

  auto *node_left = sectionize(section_left, splites - 1);
  auto *node_right = sectionize(section_right, splites - 1);
  return new node({s, s, node_left, node_right, splites});
}

bool map_generator::validSection(section s) {
  return s.width() >= 4 && s.height() >= 4 && s.ratio() < MAX_RATIO;
}

int map_generator::random(int min, int max) {
  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<> dis(min, max);
  return dis(gen);
}

std::vector<section> map_generator::generateRooms(node *n) {
  std::vector<section> results;
  if (n == nullptr) {
    return results;
  }

  if (n->left == nullptr && n->right == nullptr) {
    results.push_back(randomRoom(n));
  } else {
    auto leftRooms = generateRooms(n->left);
    auto rightRooms = generateRooms(n->right);
    results.insert(results.end(), leftRooms.begin(), leftRooms.end());
    results.insert(results.end(), rightRooms.begin(), rightRooms.end());
  }

  return results;
}

std::vector<node *> map_generator::getFromLeafs(node *n, int which) {
  std::vector<node *> results;

  if (n->fromLeaf == which) {
    results.push_back(n);
  } else {
    auto lefts = getFromLeafs(n->left, which);
    auto rights = getFromLeafs(n->right, which);
    results.insert(results.end(), lefts.begin(), lefts.end());
    results.insert(results.end(), rights.begin(), rights.end());
  }

  return results;
}

section map_generator::randomRoom(node *n) {
  section room;
  auto iter = 0;
  auto s = n->section;
  auto center = s.center();

  do {
    if (iter++ >= MAX_ITERATIONS) {
      bure::logger::error("max iterations reached in randomRoom()");
      exit(1);
    }

    auto p1x = random(s.p1.x, center.x);
    auto p1y = random(s.p1.y, center.y);
    auto p2x = random(center.x, s.p2.x);
    auto p2y = random(center.y, s.p2.y);
    room = {{p1x, p1y}, {p2x, p2y}};
  } while (!validSection(room) || !isInSection(center, room) ||
           (room.space() * 1.0) / s.space() < MAX_SPACE_IN_SECTION);

  n->room = room;
  return room;
}

bool map_generator::isInSection(bure::map_coords p, section s) {
  return (p.x >= s.p1.x && p.x <= s.p2.x && p.y >= s.p1.y && p.y <= s.p2.y);
}

void map_generator::drawPaths(node *n, int *layer, int steps, int width) {
  std::vector<section> paths;

  for (int i = steps; i > 0; i--) {
    auto nodes = getFromLeafs(n, i);

    for (auto *node : nodes) {
      auto leftCenter = node->left->section.center();
      auto rightCenter = node->right->section.center();

      if (rightCenter.y == leftCenter.y) {
        for (int x = rightCenter.x; x != leftCenter.x; x--) {
          layer[rightCenter.y * width + x] = 1;
        }
      } else {
        for (int y = rightCenter.y; y != leftCenter.y; y--) {
          layer[y * width + rightCenter.x] = 1;
        }
      }
    }
  }
}

void map_generator::drawRooms(std::vector<section> &rooms, int *layer,
                              int width) {
  for (auto s : rooms) {
    for (int i = s.p1.y; i < s.p2.y; i++) {
      for (int j = s.p1.x; j < s.p2.x; j++) {
        layer[i * width + j] = 1;
      }
    }
  }
}

unsigned int map_generator::getTile(int *layer, int x, int y, int width,
                                    int height) {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return 0;
  }

  return layer[y * width + x];
}

void map_generator::drawTiles(int *skeleton, int *result, int width, int height,
                              const std::vector<cube_tile> &cubeTiles) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      cube c = {
          getTile(skeleton, x - 1, y - 1, width, height),
          getTile(skeleton, x, y - 1, width, height),
          getTile(skeleton, x + 1, y - 1, width, height),
          getTile(skeleton, x - 1, y, width, height),
          getTile(skeleton, x, y, width, height),
          getTile(skeleton, x + 1, y, width, height),
          getTile(skeleton, x - 1, y + 1, width, height),
          getTile(skeleton, x, y + 1, width, height),
          getTile(skeleton, x + 1, y + 1, width, height),
      };

      for (const auto &ct : cubeTiles) {
        if (c == ct.tileCube) {
          result[y * width + x] = ct.tileId;
        }
      }
    }
  }
}

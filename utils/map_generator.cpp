#include "map_generator.h"
#include "utils/pathfinding.h"
#include "utils/logger.h"

#include <iostream>
#include <random>
#include <unordered_set>

#define MAX_ITERATIONS 1000

#define TILE_WATER_ID 5

#define TILE_FIELD_ID 42
#define TILE_FIELD_LEFT_ID 9
#define TILE_FIELD_RIGHT_ID 11
#define TILE_FIELD_TOP_ID 2
#define TILE_FIELD_BOTTOM_ID 18
#define TILE_INNER_CENTER 52

#define TILE_FIELD_BOTH_HORIZONTAL_ID 12
#define TILE_FIELD_BOTH_HORIZONTAL_TOP_ID 4
#define TILE_FIELD_BOTH_HORIZONTAL_BOTTOM_ID 20

#define TILE_FIELD_TOP_LEFT_ID 1
#define TILE_FIELD_TOP_RIGHT_ID 3
#define TILE_FIELD_BOTTOM_LEFT_ID 17
#define TILE_FIELD_BOTTOM_RIGHT_ID 19

#define TILE_INNER_LEFT_ID 41
#define TILE_INNER_RIGHT_ID 43
#define TILE_INNER_TOP_ID 34
#define TILE_INNER_BOTTOM_ID 50
#define TILE_INNER_TOP_LEFT_ID 33
#define TILE_INNER_TOP_RIGHT_ID 35
#define TILE_INNER_BOTTOM_LEFT_ID 49
#define TILE_INNER_BOTTOM_RIGHT_ID 51

#define TILE_INNER_CORNER_TOP_LEFT_ID 60
#define TILE_INNER_CORNER_TOP_RIGHT_ID 59
#define TILE_INNER_CORNER_BOTTOM_LEFT_ID 58
#define TILE_INNER_CORNER_BOTTOM_RIGHT_ID 57

#define TILE_INNER_CORNER_TOP_LEFT_VAR1_ID 68
#define TILE_INNER_CORNER_TOP_RIGHT_VAR1_ID 67
#define TILE_INNER_CORNER_BOTTOM_LEFT_VAR1_ID 66
#define TILE_INNER_CORNER_BOTTOM_RIGHT_VAR1_ID 65

#define TILE_INNER_CORNER_TOP_LEFT_VAR2_ID 76
#define TILE_INNER_CORNER_TOP_RIGHT_VAR2_ID 75
#define TILE_INNER_CORNER_BOTTOM_LEFT_VAR2_ID 74
#define TILE_INNER_CORNER_BOTTOM_RIGHT_VAR2_ID 73

#define TILE_INNER_CORNER_TOP_LEFT_VAR3_ID 84
#define TILE_INNER_CORNER_TOP_RIGHT_VAR3_ID 83
#define TILE_INNER_CORNER_BOTTOM_LEFT_VAR3_ID 82
#define TILE_INNER_CORNER_BOTTOM_RIGHT_VAR3_ID 81

#define TILE_OUTSIDE_CORNER_TOP_LEFT 25
#define TILE_OUTSIDE_CORNER_TOP_RIGHT 26
#define TILE_OUTSIDE_CORNER_BOTTOM_LEFT 27
#define TILE_OUTSIDE_CORNER_BOTTOM_RIGHT 28

#define TILE_DOUBLE_CORNER_BOTTOM 61
#define TILE_DOUBLE_CORNER_LEFT 62
#define TILE_DOUBLE_CORNER_TOP 63
#define TILE_DOUBLE_CORNER_RIGHT 64

#define TILE_DOUBLE_CORNER_BOTTOM_VAR1 69
#define TILE_DOUBLE_CORNER_LEFT_VAR1 70
#define TILE_DOUBLE_CORNER_TOP_VAR1 71
#define TILE_DOUBLE_CORNER_RIGHT_VAR1 72

#define TILE_DOUBLE_CORNER_BOTTOM_VAR2 77
#define TILE_DOUBLE_CORNER_LEFT_VAR2 78
#define TILE_DOUBLE_CORNER_TOP_VAR2 79
#define TILE_DOUBLE_CORNER_RIGHT_VAR2 80

#define TILE_DOUBLE_CORNER_BOTTOM_VAR3 85
#define TILE_DOUBLE_CORNER_LEFT_VAR3 86
#define TILE_DOUBLE_CORNER_TOP_VAR3 87
#define TILE_DOUBLE_CORNER_RIGHT_VAR3 88

#define TILE_DOUBLE_INNER_VERTICAL 31
#define TILE_DOUBLE_INNER_HORIZONTAL 32

#define TILE_BOTH_CORNERS_TOP 29
#define TILE_BOTH_CORNERS_BOTTOM 30

#define TILE_BORDERS_DOBLE 89
#define TILE_BORDERS_VAR1 90
#define TILE_BORDERS_VAR2 91
#define TILE_BORDERS_VAR3 92
#define TILE_BORDERS_VAR4 93

#define PADDING 10

std::unique_ptr<bure::game_map> map_generator::generate(int w, int h,
                                                        int splites) {
  auto width = w + PADDING * 2;
  auto height = h + PADDING * 2;

  auto map = std::make_unique<bure::game_map>(width, height, 32, 32);

  auto layerData = new int[height * width];
  auto pisableData = new int[height * width];

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      layerData[i * width + j] = TILE_WATER_ID;
      pisableData[i * width + j] = 0;
    }
  }

  auto s = section({{PADDING, PADDING}, {width - PADDING, height - PADDING}});
  auto tree = sectionize(s, splites);
  auto rooms = generateRooms(tree);

  drawRooms(rooms, layerData, pisableData, width);
  drawPaths(tree, layerData, pisableData, splites, width);
  drawTiles(pisableData, layerData, width, height);

  // TODO: CLEAN TREE

  bure::layer l = {0, "terrain", layerData, true};
  bure::layer pisable = {1, "pisable", pisableData, false};
  bure::tileset ts = {
      0, "terrain_tileset", "../sprites/tileset_dungeon_mod.png", 1, 8, 32};

  map->addLayer(l);
  map->addLayer(pisable);
  map->addTileset(ts);

  return map;
}

node *map_generator::sectionize(section s, int splites) {
  section section_left, section_right;
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

  auto node_left = sectionize(section_left, splites - 1);
  auto node_right = sectionize(section_right, splites - 1);
  return new node({s, s, node_left, node_right, splites});
}

bool map_generator::validSection(section s) {
  static float max_ratio = 2.22;
  return s.width() >= 4 &&
         s.height() >= 4 &&
         s.ratio() < max_ratio;
}

int map_generator::random(int min, int max) {
  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<> dis(min, max);
  return dis(gen);
}

std::vector<section> map_generator::generateRooms(node *n) {
  std::vector<section> results;

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
  } while (!validSection(room) ||
           !isInSection(center, room) ||
           static_cast<float>(room.space()) / s.space() < 0.6);

  n->room = room;
  return room;
}

bool map_generator::isInSection(bure::map_coords p, section s) {
  return (p.x >= s.p1.x && p.x <= s.p2.x && p.y >= s.p1.y && p.y <= s.p2.y);
}

void map_generator::drawPaths(node *n, int *layerData, int *pisableData,
                              int steps, int width) {
  std::vector<section> paths;

  for (int i = steps; i > 0; i--) {
    auto nodes = getFromLeafs(n, i);

    for (auto node : nodes) {
      auto leftCenter = node->left->section.center();
      auto rightCenter = node->right->section.center();

      if (rightCenter.y == leftCenter.y) {
        // horizontal split
        for (int x = rightCenter.x; x != leftCenter.x; x--) {
          layerData[rightCenter.y * width + x] = TILE_FIELD_ID;
          pisableData[rightCenter.y * width + x] = 1;
        }
      } else {
        // vertical split
        for (int y = rightCenter.y; y != leftCenter.y; y--) {
          layerData[y * width + rightCenter.x] = TILE_FIELD_ID;
          pisableData[y * width + rightCenter.x] = 1;
        }
      }
    }
  }
}

void map_generator::drawRooms(std::vector<section> &rooms, int *layerData,
                              int *pisableData, int width) {
  for (auto s : rooms) {
    for (int i = s.p1.y; i < s.p2.y; i++) {
      for (int j = s.p1.x; j < s.p2.x ; j++) {
        layerData[i * width + j] = TILE_FIELD_ID;
        pisableData[i * width + j] = 1;
      }
    }
  }
}

int map_generator::getTile(int *pisable, int x, int y, int width, int height) {
  if (x < 0 || x >= width || y < 0 || y >= height) return 0;
  else return pisable[y * width + x];
}

void map_generator::drawTiles(int *pisable, int *dst, int width, int height) {
  std::vector<cube_tile> cubeTiles;

  cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 1, 1, 1}, TILE_FIELD_ID});

  // 1 Tile
  cubeTiles.push_back({{0, 0, 0, 0, 0, 1, 0, 0, 0}, TILE_FIELD_ID});
  cubeTiles.push_back({{0, 0, 0, 0, 0, 0, 0, 1, 0}, TILE_FIELD_ID});
  cubeTiles.push_back({{0, 0, 0, 1, 0, 0, 0, 0, 0}, TILE_FIELD_ID});
  cubeTiles.push_back({{0, 1, 0, 0, 0, 0, 0, 0, 0}, TILE_FIELD_ID});
  cubeTiles.push_back({{0, 0, 0, 0, 0, 0, 0, 0, 1}, TILE_FIELD_TOP_LEFT_ID});
  cubeTiles.push_back({{0, 0, 0, 0, 0, 0, 1, 0, 0}, TILE_FIELD_TOP_RIGHT_ID});
  cubeTiles.push_back({{0, 0, 1, 0, 0, 0, 0, 0, 0}, TILE_FIELD_BOTTOM_LEFT_ID});
  cubeTiles.push_back({{1, 0, 0, 0, 0, 0, 0, 0, 0}, TILE_FIELD_BOTTOM_RIGHT_ID});
  cubeTiles.push_back({{1, 0, 0, 0, 1, 0, 0, 0, 0}, TILE_INNER_CENTER});

  // 2 Tiles
  cubeTiles.push_back({{0, 0, 0, 0, 0, 0, 0, 1, 1}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{0, 0, 0, 0, 0, 0, 1, 1, 0}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{0, 0, 0, 1, 0, 0, 1, 0, 0}, TILE_FIELD_RIGHT_ID});
  cubeTiles.push_back({{1, 0, 0, 1, 0, 0, 0, 0, 0}, TILE_FIELD_RIGHT_ID});
  cubeTiles.push_back({{0, 0, 1, 0, 0, 1, 0, 0, 0}, TILE_FIELD_LEFT_ID});
  cubeTiles.push_back({{0, 0, 0, 0, 0, 1, 0, 0, 1}, TILE_FIELD_LEFT_ID});
  cubeTiles.push_back({{0, 1, 1, 0, 0, 0, 0, 0, 0}, TILE_FIELD_BOTTOM_ID});
  cubeTiles.push_back({{1, 1, 0, 0, 0, 0, 0, 0, 0}, TILE_FIELD_BOTTOM_ID});
  cubeTiles.push_back({{0, 1, 0, 0, 1, 0, 0, 0, 0}, TILE_FIELD_ID});
  cubeTiles.push_back({{0, 0, 0, 1, 1, 0, 0, 0, 0}, TILE_FIELD_ID});
  cubeTiles.push_back({{0, 0, 0, 0, 1, 1, 0, 0, 0}, TILE_FIELD_ID});
  cubeTiles.push_back({{0, 0, 0, 0, 1, 0, 0, 1, 0}, TILE_FIELD_ID});
  cubeTiles.push_back({{1, 0, 1, 0, 0, 0, 0, 0, 0}, TILE_BOTH_CORNERS_TOP});
  cubeTiles.push_back({{0, 0, 0, 0, 0, 0, 1, 0, 1}, TILE_BOTH_CORNERS_BOTTOM});
  cubeTiles.push_back({{1, 0, 0, 0, 0, 0, 1, 0, 0}, TILE_FIELD_RIGHT_ID});
  cubeTiles.push_back({{0, 0, 1, 0, 0, 0, 0, 0, 1}, TILE_FIELD_LEFT_ID});

  // 3 Tiles
  cubeTiles.push_back({{1, 0, 0, 1, 0, 0, 1, 0, 0}, TILE_FIELD_RIGHT_ID});
  cubeTiles.push_back({{0, 0, 1, 0, 0, 1, 0, 0, 1}, TILE_FIELD_LEFT_ID});
  cubeTiles.push_back({{1, 1, 1, 0, 0, 0, 0, 0, 0}, TILE_FIELD_BOTTOM_ID});
  cubeTiles.push_back({{0, 0, 0, 0, 0, 0, 1, 1, 1}, TILE_FIELD_TOP_ID});
  // cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 0, 0, 0}, TILE_DOUBLE_INNER_HORIZONTAL});
  cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 0, 0, 0}, TILE_INNER_TOP_ID});
  cubeTiles.push_back({{0, 1, 0, 0, 1, 0, 0, 1, 0}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{0, 0, 1, 0, 0, 0, 0, 1, 1}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{1, 0, 0, 0, 0, 0, 1, 1, 0}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{0, 1, 1, 0, 0, 0, 0, 0, 1}, TILE_OUTSIDE_CORNER_TOP_RIGHT});
  cubeTiles.push_back({{1, 0, 1, 0, 0, 1, 0, 0, 0}, TILE_BORDERS_VAR1});
  cubeTiles.push_back({{1, 0, 1, 1, 0, 0, 0, 0, 0}, TILE_BORDERS_VAR2});
  cubeTiles.push_back({{1, 1, 0, 0, 0, 0, 1, 0, 0}, TILE_OUTSIDE_CORNER_TOP_LEFT});
  cubeTiles.push_back({{0, 0, 0, 0, 0, 1, 1, 0, 1}, TILE_BORDERS_VAR3});
  cubeTiles.push_back({{0, 0, 0, 1, 0, 0, 1, 0, 1}, TILE_BORDERS_VAR4});

  // 4 Tiles
  cubeTiles.push_back({{0, 0, 0, 0, 1, 1, 0, 1, 1}, TILE_INNER_TOP_LEFT_ID});
  cubeTiles.push_back({{0, 0, 0, 1, 1, 0, 1, 1, 0}, TILE_INNER_TOP_RIGHT_ID});
  // cubeTiles.push_back({{0, 1, 1, 0, 1, 1, 0, 0, 0}, TILE_INNER_BOTTOM_LEFT_ID});
  cubeTiles.push_back({{0, 1, 1, 0, 1, 1, 0, 0, 0}, TILE_INNER_LEFT_ID});
  // cubeTiles.push_back({{1, 1, 0, 1, 1, 0, 0, 0, 0}, TILE_INNER_BOTTOM_RIGHT_ID});
  cubeTiles.push_back({{1, 1, 0, 1, 1, 0, 0, 0, 0}, TILE_INNER_RIGHT_ID});
  cubeTiles.push_back({{1, 0, 1, 1, 0, 1, 0, 0, 0}, TILE_FIELD_BOTH_HORIZONTAL_ID});
  cubeTiles.push_back({{0, 0, 0, 1, 0, 1, 1, 0, 1}, TILE_FIELD_BOTH_HORIZONTAL_ID});
  cubeTiles.push_back({{0, 0, 1, 0, 0, 0, 1, 1, 1}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{1, 0, 0, 0, 0, 0, 1, 1, 1}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{1, 1, 1, 0, 0, 1, 0, 0, 0}, TILE_OUTSIDE_CORNER_TOP_RIGHT});
  cubeTiles.push_back({{1, 1, 1, 1, 0, 0, 0, 0, 0}, TILE_OUTSIDE_CORNER_TOP_LEFT});
  cubeTiles.push_back({{1, 1, 0, 0, 0, 0, 1, 1, 0}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{0, 0, 0, 0, 0, 1, 1, 1, 1}, TILE_OUTSIDE_CORNER_BOTTOM_RIGHT});
  cubeTiles.push_back({{0, 0, 0, 1, 0, 0, 1, 1, 1}, TILE_OUTSIDE_CORNER_BOTTOM_LEFT});
  cubeTiles.push_back({{1, 1, 1, 0, 0, 0, 1, 0, 0}, TILE_OUTSIDE_CORNER_TOP_LEFT});
  // cubeTiles.push_back({{0, 1, 0, 1, 1, 1, 0, 0, 0}, TILE_DOUBLE_CORNER_TOP_VAR1});
  cubeTiles.push_back({{0, 1, 0, 1, 1, 1, 0, 0, 0}, TILE_DOUBLE_CORNER_TOP});
  // cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 0, 1, 0}, TILE_DOUBLE_CORNER_BOTTOM_VAR1});
  cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 0, 1, 0}, TILE_INNER_TOP_ID});
  // cubeTiles.push_back({{0, 1, 0, 1, 1, 0, 0, 1, 0}, TILE_DOUBLE_CORNER_LEFT_VAR1});
  cubeTiles.push_back({{0, 1, 0, 1, 1, 0, 0, 1, 0}, TILE_INNER_CORNER_TOP_LEFT_VAR3_ID});
  // cubeTiles.push_back({{0, 1, 0, 0, 1, 1, 0, 1, 0}, TILE_DOUBLE_CORNER_RIGHT_VAR1});
  cubeTiles.push_back({{0, 1, 0, 0, 1, 1, 0, 1, 0}, TILE_INNER_CORNER_TOP_RIGHT_VAR3_ID});
  cubeTiles.push_back({{1, 1, 1, 0, 0, 0, 0, 0, 1}, TILE_OUTSIDE_CORNER_TOP_RIGHT});
  cubeTiles.push_back({{0, 0, 1, 0, 0, 1, 0, 1, 1}, TILE_FIELD_TOP_ID});
  // cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 0, 0, 1}, TILE_DOUBLE_INNER_HORIZONTAL});
  cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 0, 0, 1}, TILE_INNER_TOP_ID});
  // cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 1, 0, 0}, TILE_DOUBLE_INNER_HORIZONTAL});
  cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 1, 0, 0}, TILE_INNER_TOP_ID});
  // cubeTiles.push_back({{0, 0, 1, 1, 1, 1, 0, 0, 0}, TILE_DOUBLE_INNER_HORIZONTAL});
  cubeTiles.push_back({{0, 0, 1, 1, 1, 1, 0, 0, 0}, TILE_INNER_TOP_ID});
  // cubeTiles.push_back({{1, 0, 0, 1, 1, 1, 0, 0, 0}, TILE_DOUBLE_INNER_HORIZONTAL});
  cubeTiles.push_back({{1, 0, 0, 1, 1, 1, 0, 0, 0}, TILE_INNER_TOP_ID});
  cubeTiles.push_back({{0, 1, 0, 0, 1, 0, 1, 1, 0}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{0, 1, 0, 0, 1, 0, 0, 1, 1}, TILE_DOUBLE_INNER_VERTICAL});
  // cubeTiles.push_back({{1, 1, 0, 0, 1, 0, 0, 1, 0}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{1, 1, 0, 0, 1, 0, 0, 1, 0}, TILE_INNER_CORNER_BOTTOM_LEFT_VAR3_ID});
  // cubeTiles.push_back({{0, 1, 1, 0, 1, 0, 0, 1, 0}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{0, 1, 1, 0, 1, 0, 0, 1, 0}, TILE_INNER_CORNER_BOTTOM_RIGHT_VAR3_ID});
  cubeTiles.push_back({{0, 1, 1, 0, 0, 0, 0, 1, 1}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{0, 1, 1, 0, 0, 1, 0, 0, 1}, TILE_OUTSIDE_CORNER_TOP_RIGHT});
  cubeTiles.push_back({{1, 0, 1, 0, 0, 1, 0, 0, 1}, TILE_BORDERS_VAR1});
  cubeTiles.push_back({{1, 0, 1, 1, 0, 0, 1, 0, 0}, TILE_BORDERS_VAR2});
  cubeTiles.push_back({{0, 0, 1, 0, 0, 1, 1, 0, 1}, TILE_BORDERS_VAR3});
  cubeTiles.push_back({{1, 0, 0, 1, 0, 0, 1, 0, 1}, TILE_BORDERS_VAR4});
  cubeTiles.push_back({{1, 1, 0, 1, 0, 0, 1, 0, 0}, TILE_OUTSIDE_CORNER_TOP_LEFT});
  cubeTiles.push_back({{0, 1, 1, 0, 0, 1, 0, 0, 1}, TILE_OUTSIDE_CORNER_TOP_RIGHT});

  // 5 Tiles
  cubeTiles.push_back({{0, 0, 0, 0, 1, 1, 1, 1, 1}, TILE_INNER_TOP_LEFT_ID});
  cubeTiles.push_back({{0, 0, 0, 1, 1, 0, 1, 1, 1}, TILE_INNER_TOP_RIGHT_ID});
  cubeTiles.push_back({{1, 0, 0, 1, 1, 0, 1, 1, 0}, TILE_INNER_TOP_RIGHT_ID});
  // cubeTiles.push_back({{1, 1, 0, 1, 1, 0, 1, 0, 0}, TILE_INNER_BOTTOM_RIGHT_ID});
  cubeTiles.push_back({{1, 1, 0, 1, 1, 0, 1, 0, 0}, TILE_INNER_RIGHT_ID});
  // cubeTiles.push_back({{1, 1, 1, 1, 1, 0, 0, 0, 0}, TILE_INNER_BOTTOM_RIGHT_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 1, 0, 0, 0, 0}, TILE_INNER_CORNER_BOTTOM_RIGHT_ID});
  // cubeTiles.push_back({{1, 1, 1, 0, 1, 1, 0, 0, 0}, TILE_INNER_BOTTOM_LEFT_ID});
  cubeTiles.push_back({{1, 1, 1, 0, 1, 1, 0, 0, 0}, TILE_INNER_CORNER_BOTTOM_LEFT_ID});
  cubeTiles.push_back({{0, 0, 1, 0, 1, 1, 0, 1, 1}, TILE_INNER_TOP_LEFT_ID});
  // cubeTiles.push_back({{0, 1, 1, 0, 1, 1, 0, 0, 1}, TILE_INNER_BOTTOM_LEFT_ID});
  cubeTiles.push_back({{0, 1, 1, 0, 1, 1, 0, 0, 1}, TILE_INNER_LEFT_ID});
  cubeTiles.push_back({{1, 1, 1, 0, 0, 1, 0, 0, 1}, TILE_OUTSIDE_CORNER_TOP_RIGHT});
  cubeTiles.push_back({{1, 1, 1, 1, 0, 0, 1, 0, 0}, TILE_OUTSIDE_CORNER_TOP_LEFT});
  cubeTiles.push_back({{0, 0, 1, 0, 0, 1, 1, 1, 1}, TILE_OUTSIDE_CORNER_BOTTOM_RIGHT});
  cubeTiles.push_back({{1, 0, 0, 1, 0, 0, 1, 1, 1}, TILE_OUTSIDE_CORNER_BOTTOM_LEFT});
  cubeTiles.push_back({{1, 1, 1, 0, 0, 0, 0, 1, 1}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{0, 1, 1, 0, 0, 0, 1, 1, 1}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{1, 1, 0, 0, 0, 0, 1, 1, 1}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{1, 1, 1, 0, 0, 0, 1, 1, 0}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{0, 1, 0, 0, 1, 0, 1, 1, 1}, TILE_DOUBLE_INNER_VERTICAL});
  // cubeTiles.push_back({{1, 1, 1, 0, 1, 0, 0, 1, 0}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{1, 1, 1, 0, 1, 0, 0, 1, 0}, TILE_DOUBLE_CORNER_BOTTOM});
  // cubeTiles.push_back({{1, 0, 0, 1, 1, 1, 1, 0, 0}, TILE_DOUBLE_INNER_HORIZONTAL});
  cubeTiles.push_back({{1, 0, 0, 1, 1, 1, 1, 0, 0}, TILE_INNER_TOP_ID});
  // cubeTiles.push_back({{0, 0, 1, 1, 1, 1, 0, 0, 1}, TILE_DOUBLE_INNER_HORIZONTAL});
  cubeTiles.push_back({{0, 0, 1, 1, 1, 1, 0, 0, 1}, TILE_INNER_TOP_ID});
  cubeTiles.push_back({{0, 1, 1, 0, 1, 0, 1, 1, 0}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{0, 1, 1, 0, 1, 0, 0, 1, 1}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{1, 1, 0, 0, 1, 0, 0, 1, 1}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{1, 1, 0, 0, 1, 0, 1, 1, 0}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{1, 0, 1, 1, 0, 1, 1, 0, 0}, TILE_FIELD_BOTH_HORIZONTAL_ID});
  cubeTiles.push_back({{1, 0, 1, 1, 0, 1, 0, 0, 1}, TILE_FIELD_BOTH_HORIZONTAL_ID});
  cubeTiles.push_back({{1, 0, 0, 1, 0, 1, 1, 0, 1}, TILE_FIELD_BOTH_HORIZONTAL_ID});
  cubeTiles.push_back({{0, 0, 1, 1, 0, 1, 1, 0, 1}, TILE_FIELD_BOTH_HORIZONTAL_ID});
  cubeTiles.push_back({{1, 1, 0, 1, 1, 0, 0, 1, 0}, TILE_INNER_CORNER_BOTTOM_LEFT_VAR3_ID});
  cubeTiles.push_back({{1, 1, 0, 1, 1, 0, 0, 1, 0}, TILE_INNER_RIGHT_ID});
  // cubeTiles.push_back({{0, 1, 1, 0, 1, 1, 0, 1, 0}, TILE_INNER_CORNER_BOTTOM_RIGHT_VAR3_ID});
  cubeTiles.push_back({{0, 1, 1, 0, 1, 1, 0, 1, 0}, TILE_INNER_LEFT_ID});
  cubeTiles.push_back({{0, 1, 0, 1, 1, 0, 1, 1, 0}, TILE_INNER_CORNER_TOP_LEFT_VAR3_ID});
  cubeTiles.push_back({{0, 1, 0, 0, 1, 1, 0, 1, 1}, TILE_INNER_CORNER_TOP_RIGHT_VAR3_ID});
  // cubeTiles.push_back({{0, 1, 1, 1, 1, 1, 0, 0, 0}, TILE_INNER_CORNER_TOP_LEFT_VAR2_ID});
  cubeTiles.push_back({{0, 1, 1, 1, 1, 1, 0, 0, 0}, TILE_INNER_CORNER_TOP_LEFT_ID});
  // cubeTiles.push_back({{1, 1, 0, 1, 1, 1, 0, 0, 0}, TILE_INNER_CORNER_TOP_RIGHT_VAR2_ID});
  cubeTiles.push_back({{1, 1, 0, 1, 1, 1, 0, 0, 0}, TILE_INNER_CORNER_TOP_RIGHT_ID});
  // cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 0, 1, 1}, TILE_INNER_CORNER_BOTTOM_LEFT_VAR2_ID});
  cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 0, 1, 1}, TILE_INNER_TOP_ID});
  // cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 1, 1, 0}, TILE_INNER_CORNER_BOTTOM_RIGHT_VAR2_ID});
  cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 1, 1, 0}, TILE_INNER_TOP_ID});
  cubeTiles.push_back({{0, 1, 0, 1, 1, 1, 0, 1, 0}, TILE_DOUBLE_CORNER_LEFT_VAR1});
  cubeTiles.push_back({{0, 1, 0, 0, 1, 1, 1, 1, 0}, TILE_DOUBLE_CORNER_RIGHT_VAR1});
  cubeTiles.push_back({{1, 1, 1, 0, 0, 0, 1, 0, 1}, TILE_BORDERS_DOBLE});
  cubeTiles.push_back({{1, 0, 0, 1, 1, 1, 0, 0, 1}, TILE_INNER_TOP_ID});


  // 6 Tiles
  cubeTiles.push_back({{0, 0, 0, 1, 1, 1, 1, 1, 1}, TILE_INNER_TOP_ID});
  cubeTiles.push_back({{0, 1, 1, 0, 1, 1, 0, 1, 1}, TILE_INNER_LEFT_ID});
  cubeTiles.push_back({{1, 1, 0, 1, 1, 0, 1, 1, 0}, TILE_INNER_RIGHT_ID});
  // cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 0, 0, 0}, TILE_INNER_BOTTOM_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 0, 0, 0}, TILE_FIELD_ID});
  cubeTiles.push_back({{1, 0, 1, 1, 0, 1, 1, 0, 1}, TILE_FIELD_BOTH_HORIZONTAL_ID});
  cubeTiles.push_back({{1, 1, 1, 0, 0, 0, 1, 1, 1}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 0, 0, 1, 1, 0}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{1, 0, 1, 1, 1, 1, 0, 0, 1}, TILE_DOUBLE_INNER_HORIZONTAL});
  cubeTiles.push_back({{1, 0, 1, 1, 1, 1, 1, 0, 0}, TILE_DOUBLE_INNER_HORIZONTAL});
  // cubeTiles.push_back({{1, 0, 0, 1, 1, 1, 1, 0, 1}, TILE_DOUBLE_INNER_HORIZONTAL});
  cubeTiles.push_back({{1, 0, 0, 1, 1, 1, 1, 0, 1}, TILE_INNER_TOP_ID});
  // cubeTiles.push_back({{0, 0, 1, 1, 1, 1, 1, 0, 1}, TILE_DOUBLE_INNER_HORIZONTAL});
  cubeTiles.push_back({{0, 0, 1, 1, 1, 1, 1, 0, 1}, TILE_INNER_TOP_ID});
  // cubeTiles.push_back({{1, 1, 1, 0, 1, 0, 1, 1, 0}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{1, 1, 1, 0, 1, 0, 1, 1, 0}, TILE_DOUBLE_CORNER_BOTTOM});
  // cubeTiles.push_back({{1, 1, 0, 0, 1, 0, 1, 1, 1}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{1, 1, 0, 0, 1, 0, 1, 1, 1}, TILE_INNER_CORNER_BOTTOM_LEFT_VAR3_ID});
  // cubeTiles.push_back({{1, 1, 1, 0, 1, 0, 0, 1, 1}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{1, 1, 1, 0, 1, 0, 0, 1, 1}, TILE_DOUBLE_CORNER_BOTTOM});
  // cubeTiles.push_back({{0, 1, 1, 0, 1, 0, 1, 1, 1}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{0, 1, 1, 0, 1, 0, 1, 1, 1}, TILE_INNER_CORNER_BOTTOM_RIGHT_VAR3_ID});
  // cubeTiles.push_back({{1, 0, 0, 1, 1, 1, 1, 1, 0}, TILE_INNER_CORNER_BOTTOM_RIGHT_VAR2_ID});
  cubeTiles.push_back({{1, 0, 0, 1, 1, 1, 1, 1, 0}, TILE_INNER_TOP_ID});
  // cubeTiles.push_back({{0, 0, 1, 1, 1, 1, 0, 1, 1}, TILE_INNER_CORNER_BOTTOM_LEFT_VAR2_ID});
  cubeTiles.push_back({{0, 0, 1, 1, 1, 1, 0, 1, 1}, TILE_INNER_TOP_ID});
  // cubeTiles.push_back({{1, 1, 0, 1, 1, 1, 1, 0, 0}, TILE_INNER_CORNER_TOP_RIGHT_VAR2_ID});
  cubeTiles.push_back({{1, 1, 0, 1, 1, 1, 1, 0, 0}, TILE_INNER_CORNER_TOP_RIGHT_ID});
  cubeTiles.push_back({{0, 1, 1, 1, 1, 1, 0, 0, 1}, TILE_INNER_CORNER_TOP_LEFT_VAR2_ID});
  cubeTiles.push_back({{0, 1, 0, 1, 1, 0, 1, 1, 1}, TILE_INNER_CORNER_TOP_LEFT_VAR3_ID});
  cubeTiles.push_back({{0, 1, 0, 0, 1, 1, 1, 1, 1}, TILE_INNER_CORNER_TOP_RIGHT_VAR3_ID});
  // cubeTiles.push_back({{1, 1, 1, 1, 1, 0, 0, 1, 0}, TILE_INNER_CORNER_BOTTOM_LEFT_VAR3_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 1, 0, 0, 1, 0}, TILE_INNER_CORNER_BOTTOM_RIGHT_ID});
  // cubeTiles.push_back({{1, 1, 1, 0, 1, 1, 0, 1, 0}, TILE_INNER_CORNER_BOTTOM_RIGHT_VAR3_ID});
  cubeTiles.push_back({{1, 1, 1, 0, 1, 1, 0, 1, 0}, TILE_INNER_CORNER_BOTTOM_LEFT_ID});
  cubeTiles.push_back({{0, 1, 1, 1, 1, 1, 0, 1, 0}, TILE_DOUBLE_CORNER_LEFT_VAR2});
  cubeTiles.push_back({{1, 1, 0, 1, 1, 1, 0, 1, 0}, TILE_DOUBLE_CORNER_RIGHT_VAR2});
  cubeTiles.push_back({{0, 1, 0, 1, 1, 1, 0, 1, 1}, TILE_DOUBLE_CORNER_TOP});
  cubeTiles.push_back({{0, 1, 0, 1, 1, 1, 1, 1, 0}, TILE_DOUBLE_CORNER_TOP});
  cubeTiles.push_back({{0, 1, 1, 1, 1, 0, 1, 1, 0}, TILE_INNER_CORNER_TOP_LEFT_VAR3_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 0, 1, 1, 0, 0}, TILE_FIELD_BOTH_HORIZONTAL_TOP_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 0, 1, 0, 0, 1}, TILE_FIELD_BOTH_HORIZONTAL_TOP_ID});
  cubeTiles.push_back({{1, 0, 1, 0, 0, 1, 1, 1, 1}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{1, 0, 1, 1, 0, 0, 1, 1, 1}, TILE_FIELD_TOP_ID});

  // 7 Tiles
  cubeTiles.push_back({{0, 0, 1, 1, 1, 1, 1, 1, 1}, TILE_INNER_TOP_ID});
  cubeTiles.push_back({{1, 0, 0, 1, 1, 1, 1, 1, 1}, TILE_INNER_TOP_ID});
  cubeTiles.push_back({{0, 1, 1, 0, 1, 1, 1, 1, 1}, TILE_INNER_LEFT_ID});
  // cubeTiles.push_back({{1, 1, 1, 0, 1, 1, 0, 1, 1}, TILE_INNER_LEFT_ID});
  cubeTiles.push_back({{1, 1, 1, 0, 1, 1, 0, 1, 1}, TILE_INNER_CORNER_BOTTOM_LEFT_ID});
  cubeTiles.push_back({{1, 1, 0, 1, 1, 0, 1, 1, 1}, TILE_INNER_RIGHT_ID});
  // cubeTiles.push_back({{1, 1, 1, 1, 1, 0, 1, 1, 0}, TILE_INNER_RIGHT_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 1, 0, 1, 1, 0}, TILE_INNER_CORNER_BOTTOM_RIGHT_ID});
  //cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 1, 0, 0}, TILE_INNER_BOTTOM_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 1, 0, 0}, TILE_FIELD_ID});
  // cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 0, 0, 1}, TILE_INNER_BOTTOM_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 0, 0, 1}, TILE_FIELD_ID});
  cubeTiles.push_back({{1, 1, 0, 1, 1, 1, 0, 1, 1}, TILE_FIELD_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 0, 1, 1, 0, 1}, TILE_FIELD_BOTH_HORIZONTAL_TOP_ID});
  cubeTiles.push_back({{1, 0, 1, 1, 0, 1, 1, 1, 1}, TILE_FIELD_BOTH_HORIZONTAL_BOTTOM_ID});
  cubeTiles.push_back({{1, 1, 1, 0, 0, 1, 1, 1, 1}, TILE_FIELD_TOP_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 0, 0, 1, 1, 1}, TILE_FIELD_TOP_ID});
  // cubeTiles.push_back({{1, 1, 0, 1, 1, 1, 1, 1, 0}, TILE_DOUBLE_CORNER_RIGHT});
  cubeTiles.push_back({{1, 1, 0, 1, 1, 1, 1, 1, 0}, TILE_INNER_CORNER_TOP_RIGHT_ID});
  // cubeTiles.push_back({{0, 1, 1, 1, 1, 1, 0, 1, 1}, TILE_DOUBLE_CORNER_LEFT});
  cubeTiles.push_back({{0, 1, 1, 1, 1, 1, 0, 1, 1}, TILE_INNER_CORNER_TOP_LEFT_ID});
  cubeTiles.push_back({{0, 1, 0, 1, 1, 1, 1, 1, 1}, TILE_DOUBLE_CORNER_TOP});
  // cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 0, 1, 0}, TILE_DOUBLE_CORNER_BOTTOM});
  cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 0, 1, 0}, TILE_FIELD_ID});
  // cubeTiles.push_back({{1, 1, 1, 0, 1, 0, 1, 1, 1}, TILE_DOUBLE_INNER_VERTICAL});
  cubeTiles.push_back({{1, 1, 1, 0, 1, 0, 1, 1, 1}, TILE_DOUBLE_CORNER_BOTTOM});
  // cubeTiles.push_back({{1, 0, 1, 1, 1, 1, 1, 0, 1}, TILE_DOUBLE_INNER_HORIZONTAL});
  cubeTiles.push_back({{1, 0, 1, 1, 1, 1, 1, 0, 1}, TILE_INNER_TOP_ID});

  // 8 Tiles
  cubeTiles.push_back({{1, 1, 0, 1, 1, 1, 1, 1, 1}, TILE_INNER_CORNER_TOP_RIGHT_ID});
  // cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 1, 1, 0}, TILE_INNER_CORNER_BOTTOM_RIGHT_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 1, 1, 0}, TILE_FIELD_ID});
  cubeTiles.push_back({{0, 1, 1, 1, 1, 1, 1, 1, 1}, TILE_INNER_CORNER_TOP_LEFT_ID});
  // cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 0, 1, 1}, TILE_INNER_CORNER_BOTTOM_LEFT_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 0, 1, 1}, TILE_FIELD_ID});
  // cubeTiles.push_back({{1, 1, 1, 1, 1, 0, 1, 1, 1}, TILE_INNER_RIGHT_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 1, 0, 1, 1, 1}, TILE_INNER_CORNER_BOTTOM_RIGHT_ID});
  // cubeTiles.push_back({{1, 1, 1, 0, 1, 1, 1, 1, 1}, TILE_INNER_LEFT_ID});
  cubeTiles.push_back({{1, 1, 1, 0, 1, 1, 1, 1, 1}, TILE_INNER_CORNER_BOTTOM_LEFT_ID});
  cubeTiles.push_back({{1, 0, 1, 1, 1, 1, 1, 1, 1}, TILE_INNER_TOP_ID});
  // cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 1, 0, 1}, TILE_INNER_BOTTOM_ID});
  cubeTiles.push_back({{1, 1, 1, 1, 1, 1, 1, 0, 1}, TILE_FIELD_ID});

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      cube c = {
        getTile(pisable, x - 1, y - 1, width, height),
        getTile(pisable, x, y - 1, width, height),
        getTile(pisable, x + 1, y - 1, width, height),
        getTile(pisable, x - 1, y, width, height),
        getTile(pisable, x, y, width, height),
        getTile(pisable, x + 1, y, width, height),
        getTile(pisable, x - 1, y + 1, width, height),
        getTile(pisable, x, y + 1, width, height),
        getTile(pisable, x + 1, y + 1, width, height),
      };

      for (const auto & ct : cubeTiles) {
        if (c == ct.tileCube) {
          dst[y * width + x] = ct.tileId;
        }
      }
    }
  }
}

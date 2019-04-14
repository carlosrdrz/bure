#include "map_generator.h"
#include "utils/pathfinding.h"
#include "utils/logger.h"

#include <iostream>
#include <random>
#include <unordered_set>

#define MAX_ITERATIONS 1000

#define TILE_WATER_ID 46
#define TILE_FIELD_ID 42
#define TILE_FIELD_LEFT_ID 9
#define TILE_FIELD_RIGHT_ID 11
#define TILE_FIELD_TOP_ID 2
#define TILE_FIELD_BOTTOM_ID 18
#define TILE_FIELD_TOP_LEFT_ID 1
#define TILE_FIELD_TOP_RIGHT_ID 3
#define TILE_FIELD_BOTTOM_LEFT 17
#define TILE_FIELD_BOTTOM_RIGHT 19
#define TILE_INNER_LEFT_ID 41
#define TILE_INNER_RIGHT_ID 43
#define TILE_INNER_TOP_ID 34
#define TILE_INNER_BOTTOM_ID 50
#define TILE_INNER_TOP_LEFT_ID 33
#define TILE_INNER_TOP_RIGHT_ID 35
#define TILE_INNER_BOTTOM_LEFT_ID 49
#define TILE_INNER_BOTTOM_RIGHT_ID 51

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
      layerData[i * width + j] = 0;
      pisableData[i * width + j] = 0;
    }
  }

  auto s = section({{PADDING, PADDING}, {width - PADDING, height - PADDING}});
  auto tree = sectionize(s, splites);
  auto rooms = generateRooms(tree);

  drawRooms(rooms, layerData, pisableData, width);

  // draw water
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (layerData[i * width + j] == 0) {
        layerData[i * width + j] = TILE_WATER_ID;
      }
    }
  }

  drawPaths(tree, layerData, pisableData, splites, width);

  // TODO: CLEAN TREE

  bure::layer l = {0, "terrain", layerData, true};
  bure::layer pisable = {1, "pisable", pisableData, false};
  bure::tileset ts = {
      0, "terrain_tileset", "../sprites/tileset_dungeon.png", 1, 8, 32};

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
    int x, y;

    x = s.p1.x;
    for (y = s.p1.y + 1; y < s.p2.y; y++) {
      layerData[y * width + x] = TILE_FIELD_LEFT_ID;
    }

    x = s.p2.x;
    for (y = s.p1.y + 1; y < s.p2.y; y++) {
      layerData[y * width + x] = TILE_FIELD_RIGHT_ID;
    }

    y = s.p1.y;
    for (x = s.p1.x + 1; x < s.p2.x; x++) {
      layerData[y * width + x] = TILE_FIELD_TOP_ID;
    }

    y = s.p2.y;
    for (x = s.p1.x + 1; x < s.p2.x; x++) {
      layerData[y * width + x] = TILE_FIELD_BOTTOM_ID;
    }

    // INNER
    x = s.p1.x + 1;
    for (y = s.p1.y + 1; y < s.p2.y; y++) {
      layerData[y * width + x] = TILE_INNER_LEFT_ID;
      pisableData[y * width + x] = 1;
    }

    x = s.p2.x - 1;
    for (y = s.p1.y + 1; y < s.p2.y; y++) {
      layerData[y * width + x] = TILE_INNER_RIGHT_ID;
      pisableData[y * width + x] = 1;
    }

    y = s.p1.y + 1;
    for (x = s.p1.x + 1; x < s.p2.x; x++) {
      layerData[y * width + x] = TILE_INNER_TOP_ID;
      pisableData[y * width + x] = 1;
    }

    y = s.p2.y - 1;
    for (x = s.p1.x + 1; x < s.p2.x; x++) {
      layerData[y * width + x] = TILE_INNER_BOTTOM_ID;
      pisableData[y * width + x] = 1;
    }

    for (int i = s.p1.y + 2; i < s.p2.y - 1; i++) {
      for (int j = s.p1.x + 2; j < s.p2.x - 1; j++) {
        layerData[i * width + j] = TILE_FIELD_ID;
        pisableData[i * width + j] = 1;
      }
    }

    layerData[s.p1.y * width + s.p1.x] = TILE_FIELD_TOP_LEFT_ID;
    layerData[s.p2.y * width + s.p1.x] = TILE_FIELD_BOTTOM_LEFT;
    layerData[s.p1.y * width + s.p2.x] = TILE_FIELD_TOP_RIGHT_ID;
    layerData[s.p2.y * width + s.p2.x] = TILE_FIELD_BOTTOM_RIGHT;

    // INNER
    layerData[(s.p1.y + 1) * width + s.p1.x + 1] = TILE_INNER_TOP_LEFT_ID;
    pisableData[(s.p1.y + 1) * width + s.p1.x + 1] = 1;
    layerData[(s.p2.y - 1) * width + s.p1.x + 1] = TILE_INNER_BOTTOM_LEFT_ID;
    pisableData[(s.p2.y - 1) * width + s.p1.x + 1] = 1;
    layerData[(s.p1.y + 1) * width + s.p2.x - 1] = TILE_INNER_TOP_RIGHT_ID;
    pisableData[(s.p1.y + 1) * width + s.p2.x - 1] = 1;
    layerData[(s.p2.y - 1) * width + s.p2.x - 1] = TILE_INNER_BOTTOM_RIGHT_ID;
    pisableData[(s.p2.y - 1) * width + s.p2.x - 1] = 1;
  }
}

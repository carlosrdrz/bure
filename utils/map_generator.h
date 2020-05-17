#pragma once

#include <memory>
#include <unordered_map>

#include "game_map.h"

#define NO_RATIO 9999999

struct section {
  bure::map_coords p1, p2;

  int width() const { return p2.x - p1.x; }
  int height() const { return p2.y - p1.y; }
  bure::map_coords center() const {
    auto x = p1.x + ((p2.x - p1.x) / 2);
    auto y = p1.y + ((p2.y - p1.y) / 2);
    return {x, y};
  }
  int space() const { return width() * height(); }
  int ratio() const {
    if (width() == 0 || height() == 0) {
      return NO_RATIO;
    }

    auto bigger = std::max(width(), height());
    auto smaller = std::min(width(), height());
    return bigger / smaller;
  }
};

struct node {
  struct section section, room;
  struct node *left, *right;
  int fromLeaf;
};

struct cube {
  unsigned int tl, t, tr, l, c, r, bl, b, br;

  bool operator==(const struct cube &rhs) const {
    return (this->tl == rhs.tl && this->t == rhs.t && this->tr == rhs.tr &&
            this->l == rhs.l && this->c == rhs.c && this->r == rhs.r &&
            this->bl == rhs.bl && this->b == rhs.b && this->br == rhs.br);
  }
};

struct cube_tile {
  struct cube tileCube;
  int tileId;
};

class map_generator {
 public:
  map_generator(const std::string &file);

  std::unique_ptr<bure::game_map> generate(int width, int height);

  static int random(int min, int max);

 private:
  std::unordered_map<std::string, int> _tiles;
  std::vector<cube_tile> _cubeTiles;
  int _splites = 3;

  static void drawPaths(node *n, int *layer, int steps, int width);
  static void drawRooms(std::vector<section> &rooms, int *layer, int width);
  static void drawTiles(int *skeleton, int *result, int width, int height,
                        const std::vector<cube_tile> &cubeTiles);

  static node *sectionize(section s, int splites);
  static section randomRoom(node *n);
  static std::vector<section> generateRooms(node *n);
  static std::vector<node *> getFromLeafs(node *n, int which);
  static bool validSection(section s);
  static bool isInSection(bure::map_coords p, section s);
  static unsigned int getTile(int *layer, int x, int y, int width, int height);
};

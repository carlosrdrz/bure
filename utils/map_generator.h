#include "game_map.h"

struct section {
  bure::map_coords p1, p2;

  int width() { return p2.x - p1.x; }
  int height() { return p2.y - p1.y; }
  bure::map_coords center() {
    auto x = p1.x + ((p2.x - p1.x) / 2);
    auto y = p1.y + ((p2.y - p1.y) / 2);
    return {x, y};
  }
  int space() { return width() * height(); }
  int ratio() {
    if (width() == 0 || height() == 0) return 9999999;

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

class map_generator {
 public:
  static int random(int min, int max);
  static std::unique_ptr<bure::game_map> generate(int width, int height,
                                                  int splites);

 private:
  static node *sectionize(section s, int splites);
  static section randomRoom(node *n);
  static std::vector<section> generateRooms(node *n);
  static std::vector<node *> getFromLeafs(node *n, int which);
  static bool validSection(section s);
  static bool isInSection(bure::map_coords p, section s);
  static void drawPaths(node *n, int *layerData, int *pisableData, int steps,
                        int width);
  static void drawRooms(std::vector<section> &rooms, int *layerData,
                        int *pisableData, int width);
};

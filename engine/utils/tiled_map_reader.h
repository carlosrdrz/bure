#include <memory>
#include <string>

#include "game_map.h"

class tiled_map_reader {
 public:
  static std::unique_ptr<bure::game_map> read(std::string file);
};

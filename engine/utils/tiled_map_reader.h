#include "game_map.h"

#include <memory>
#include <string>

class tiled_map_reader {
 public:
  static std::unique_ptr<bure::game_map> read(std::string file);
};

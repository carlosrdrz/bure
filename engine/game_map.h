#pragma once

#include <fstream>
#include <iostream>
#include <list>
#include <string>

namespace bure {

class game_map {
 public:
  int width, height;

  int *mapa;
  int *elementos;
  bool *pisable;

  explicit game_map(std::string basePath, std::string archivo);
  ~game_map();

  bool comprobarTilePisable(int x, int y);
};

}  // namespace bure

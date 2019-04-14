#include <gtest/gtest.h>

#include "../utils/map_generator.h"

TEST(MapGeneratorTest, GeneratesMaps) {
  auto maxIterations = 15;

  for (auto i = 0; i <= maxIterations; i++) {
    map_generator::generate(48, 48, 3);
  }
}

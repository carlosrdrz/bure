#include <gtest/gtest.h>
#include <memory>

#include "../utils/map_generator.h"

TEST(MapGeneratorTest, GeneratesMaps) {
  auto maxIterations = 15;
  auto mapGenerator =
      std::make_unique<map_generator>("resources/maps/generator_config.xml");

  for (auto i = 0; i <= maxIterations; i++) {
    mapGenerator->generate(48, 48);
  }
}

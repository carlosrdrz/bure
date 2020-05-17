#include <gtest/gtest.h>
#include <ui/container.h>

TEST(UserInterfaceTest, LeaksMemory) {
  std::map<int, std::list<int>> mapTest;
  mapTest[0].push_back(3);
}

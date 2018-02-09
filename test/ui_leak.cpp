#include <gtest/gtest.h>

#include <ui/container.h>

TEST(UserInterfaceTest, LeaksMemory) {
    auto container = ui::container();

    auto button = ui::button("test_button");
    button.set(0, 0, 20, 20);

    container.add(button);
}

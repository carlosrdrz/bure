cmake_minimum_required(VERSION 3.9)
project(bure-engine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(./engine/test/lib/googletest-1.8.0/googletest)

include(FindPackageHandleStandardArgs)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(LibXMLPP REQUIRED)
find_package(Glib REQUIRED)

enable_testing()

include_directories(${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${GLIB_INCLUDE_DIRS}
        ${LibXMLPP_INCLUDE_DIRS}
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        engine)

add_library(bure-ui
  engine/ui/ui_manager.cpp
  engine/ui/ui_renderer.cpp
  engine/ui/container.cpp
  engine/ui/input_box.cpp
  engine/ui/label.cpp
  engine/ui/selector.cpp
  engine/ui/button.cpp
  engine/ui/widget.cpp)

add_library(bure-engine
  engine/systems/system.cpp
  engine/systems/drawing_system.cpp
  engine/entities/entity.cpp
  engine/components/map_position_component.cpp
  engine/components/position_component.cpp
  engine/components/sprite_component.cpp
  engine/components/animation_component.cpp
  engine/components/solid_component.cpp
  engine/types.cpp
  engine/engine.cpp
  engine/utils/config.cpp
  engine/utils/logger.cpp
  engine/utils/pathfinding.cpp
  engine/utils/tiled_map_reader.cpp
  engine/graphics.cpp
  engine/game_map.cpp
  engine/event_manager.cpp
  engine/renderer.cpp
  engine/entity_renderer.cpp
  engine/game_map_renderer.cpp
  engine/game.cpp
  backward.cpp)

add_library(client-lib
  example_game.cpp
  hud_renderer.cpp
  components/movement_component.cpp
  components/stats_component.cpp
  systems/movement_system.cpp
  utils/map_generator.cpp
  entities/fire_entity.cpp
  entities/character_entity.cpp
  entities/player_entity.cpp
  entities/enemy_entity.cpp
  entities/background_entity.cpp)

add_executable(client
  main.cpp)

add_executable(client-test
  test/map_generator_test.cpp)

target_link_libraries(client
  client-lib
  bure-engine
  bure-ui
  ${SDL2_LIBRARIES}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2TTF_LIBRARY}
  ${GLIB_LIBRARY}
  ${LibXMLPP_LIBRARIES})

target_link_libraries(client-test
  client-lib
  bure-engine
  gtest
  gtest_main
  ${LibXMLPP_LIBRARIES})

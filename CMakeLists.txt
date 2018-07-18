cmake_minimum_required(VERSION 3.9)
project(bure-engine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(LibXMLPP REQUIRED)

enable_testing()

include_directories(${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${LibXMLPP_INCLUDE_DIRS})

add_library(bure-ui
  engine/ui/container.cpp
  engine/ui/input_box.cpp
  engine/ui/label.cpp
  engine/ui/selector.cpp
  engine/ui/button.h
  engine/ui/image.h
  engine/ui/widget.h)

add_library(bure-engine
  engine/system.cpp
  engine/entity.cpp
  engine/engine.cpp
  engine/utils/config.cpp
  engine/utils/error.cpp
  engine/graphics.cpp
  engine/ui_manager.cpp
  engine/game_map.cpp
  engine/event_manager.cpp
  engine/event_manager.h
  engine/events/keyboard_event.h
  engine/events/event.h)

add_executable(client
  main.cpp
  game.cpp
  player.cpp)

target_link_libraries(client bure-engine bure-ui)
target_link_libraries(client
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_NET_LIBRARIES}
        ${SDL2TTF_LIBRARY}
        ${LibXMLPP_LIBRARIES})

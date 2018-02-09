cmake_minimum_required(VERSION 3.9)
project(bure)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(LibXml2 REQUIRED)

add_subdirectory(test)
add_subdirectory(ui)

include_directories(${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIR})

add_library(bure-lib
        config.cpp
        error.cpp
        game.cpp
        graphics.cpp
        player.cpp
        user_interface.cpp
        map.cpp)

add_executable(bure main.cpp)

target_link_libraries(bure bure-ui bure-lib)
target_link_libraries(bure
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_NET_LIBRARIES}
        ${SDL2TTF_LIBRARY}
        ${LIBXML2_LIBRARIES})

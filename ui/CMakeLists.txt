cmake_minimum_required(VERSION 3.9)
project(bure-ui)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/")

include_directories(
        ${bure_SOURCE_DIR}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIR})

add_library(bure-ui container.cpp input_box.cpp label.cpp selector.cpp button.h image.h widget.h)

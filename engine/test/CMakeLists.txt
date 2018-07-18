cmake_minimum_required(VERSION 3.9)
project(bure-test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/")

add_subdirectory(./lib/googletest-1.8.0/googletest)

enable_testing()

include_directories(
        ${bure_SOURCE_DIR}
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIR})

add_executable(bure-test ui_leak.cpp)

target_link_libraries(bure-test bure-ui gtest gtest_main)
target_link_libraries(bure-test
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_NET_LIBRARIES}
        ${SDL2TTF_LIBRARY}
        ${LIBXML2_LIBRARIES})

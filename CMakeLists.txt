cmake_minimum_required(VERSION 3.9)
project(client)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_definitions(-D__LOCAL_RESOURCES__
                -D__LOCAL_NETWORK__)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(LibXml2 REQUIRED)


include_directories(${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIR})

file(GLOB sources ./*.cpp ./GUI/*.cpp ./GUI/button.h ./GUI/image.h ./GUI/widget.h)

add_executable(client
        ${sources})

target_link_libraries(client
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_NET_LIBRARIES}
        ${SDL2TTF_LIBRARY}
        ${LIBXML2_LIBRARIES})

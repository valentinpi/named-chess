#Prerequirements
cmake_minimum_required(VERSION 3.15)
project(named-chess)
find_package(SDL2 REQUIRED)

# Resources
file(COPY ../res/board.jpg
    DESTINATION ${CMAKE_BINARY_DIR}/res)
file(COPY ../res/pieces.png
    DESTINATION ${CMAKE_BINARY_DIR}/res)

# Build
set(source "")
#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
aux_source_directory(${CMAKE_SOURCE_DIR} source)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(named-chess ${source} ${CMAKE_SOURCE_DIR})
target_link_libraries(named-chess ${SDL2_LIBRARIES} -lSDL2_image)

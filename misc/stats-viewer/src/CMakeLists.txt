project(stats-viewer)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
add_executable(stats-viewer main.cpp)

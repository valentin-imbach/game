
cmake_minimum_required(VERSION 3.0)

# set the project name
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# add executables
set(SOURCES
src/Window.cpp
src/GAme.cpp
src/main.cpp)

find_package(SDL2 REQUIRED)
add_executable(Game ${SOURCES})
target_link_libraries(Game PRIVATE SDL2::SDL2)

#target_link_libraries(Game ${LIBRARIES})

target_compile_options(Game PRIVATE)

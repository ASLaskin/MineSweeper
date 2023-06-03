cmake_minimum_required(VERSION 3.14)
project(Minesweeper_Spring2023)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Board.cpp Board.h Tile.cpp Tile.h)
add_executable(Minesweeper_Spring2023 ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(Minesweeper_Spring2023 sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
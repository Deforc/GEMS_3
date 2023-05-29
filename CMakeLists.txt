cmake_minimum_required(VERSION 3.25)
project(GEMS_3)
add_executable(GEMS_3 main.cpp Gem.cpp Gem.h Engine.cpp Engine.h Game.cpp Game.h Bomb.cpp Bomb.h)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)

find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/SFML/include)
target_link_libraries(GEMS_3 sfml-system sfml-window sfml-graphics sfml-audio)

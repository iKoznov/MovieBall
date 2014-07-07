# Standard setup
cmake_minimum_required (VERSION 2.8)
project (MovieBall)

message(CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH})
message(CMAKE_ROOT = ${CMAKE_ROOT})

set(CMAKE_MODULE_PATH /usr/local/share/SFML/cmake/Modules/)
find_package(Boost REQUIRED)
find_package(SFML COMPONENTS graphics window system REQUIRED)

# Includes
include_directories (${PROJECT_SOURCE_DIR})

# Source files
set (SRC
	main.cpp
)

# Library
add_executable (movieball ${SRC})
source_group(\\ FILES ${SRC})

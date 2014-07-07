# Standard setup
cmake_minimum_required (VERSION 2.8)
project (MovieBall)

find_package(Boost REQUIRED)

# Includes
include_directories (${PROJECT_SOURCE_DIR})

# Source files
set (SRC
	main.cpp
)

# Library
add_executable (movieball ${SRC})
source_group(\\ FILES ${SRC})

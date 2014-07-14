# Standard setup
cmake_minimum_required (VERSION 2.8)
project (MovieBall)

include( CTest )

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

find_package(Boost COMPONENTS system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

find_package(SFML COMPONENTS graphics window system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

message(CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH})
message(CMAKE_ROOT = ${CMAKE_ROOT})
message(SFML_LIBRARIES = ${SFML_LIBRARIES})
message(Boost_LIBRARIES = ${Boost_LIBRARIES})
message(GLM_INCLUDE_DIRS = ${GLM_INCLUDE_DIRS})

enable_testing()
add_subdirectory(engine)
include_directories("${engine_BINARY_DIR}")
message(engine_BINARY_DIR = ${engine_BINARY_DIR})

# Includes
include_directories (${PROJECT_SOURCE_DIR})

# Source files
set (SRC
	main.cpp
	MovieBallSFML.cpp
	MovieBallSFML.hpp
)

# Library
add_executable (movieball ${SRC})
target_link_libraries(movieball engine ${OPENGL_gl_LIBRARY} ${SFML_LIBRARIES} ${Boost_LIBRARIES})
source_group(\\ FILES ${SRC})

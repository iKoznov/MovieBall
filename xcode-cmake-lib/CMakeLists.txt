# Standard setup
cmake_minimum_required (VERSION 2.8)
project (MovieBall)

include( CTest )

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

find_package(Boost COMPONENTS system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

find_package(GLEW)
include_directories(${GLEW_INCLUDE_DIRS})
add_definitions(${GLEW_DEFINITIONS})
if ( ${GLEW_FOUND} )
	add_definitions( -DGLEW_FOUND=1 )
endif()

find_package(SFML COMPONENTS graphics window system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
add_definitions(${SFML_DEFINITIONS})

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
add_definitions(${GLM_DEFINITIONS})

message(CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH})
message(CMAKE_ROOT = ${CMAKE_ROOT})
message(SFML_LIBRARIES = ${SFML_LIBRARIES})
message(Boost_LIBRARIES = ${Boost_LIBRARIES})
message(GLM_INCLUDE_DIRS = ${GLM_INCLUDE_DIRS})
message(GLEW_DEFINITIONS = ${GLEW_DEFINITIONS})
message(OPENGL_DEFINITIONS = ${OPENGL_DEFINITIONS})
message(SFML_DEFINITIONS = ${SFML_DEFINITIONS})

if ( ${CMAKE_COMPILER_IS_GNUCXX} )
	set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11 )
endif()

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
target_link_libraries(movieball engine ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${SFML_LIBRARIES} ${Boost_LIBRARIES})
source_group(\\ FILES ${SRC})

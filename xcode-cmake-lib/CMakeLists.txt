# Standard setup
cmake_minimum_required (VERSION 2.8)
project (MovieBall)

find_package(Boost COMPONENTS system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

set(CMAKE_MODULE_PATH "/usr/local/share/SFML/cmake/Modules/")
find_package(SFML COMPONENTS graphics window system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/")
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

message(CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH})
message(CMAKE_ROOT = ${CMAKE_ROOT})
message(SFML_LIBRARIES = ${SFML_LIBRARIES})
message(Boost_LIBRARIES = ${Boost_LIBRARIES})
message(GLM_INCLUDE_DIRS = ${GLM_INCLUDE_DIRS})

add_subdirectory(engine)

# Includes
include_directories (${PROJECT_SOURCE_DIR})

# Source files
set (SRC
	main.cpp
)

# Library
add_executable (movieball ${SRC})
target_link_libraries(movieball engine ${OPENGL_gl_LIBRARY} ${SFML_LIBRARIES} ${Boost_LIBRARIES})
source_group(\\ FILES ${SRC})

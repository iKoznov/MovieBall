# Standard setup
cmake_minimum_required (VERSION 2.8)
project (engine CXX C)

# The version number.
set (PROJECT_VERSION_MAJOR 1)
set (PROJECT_VERSION_MINOR 0)
file (RELATIVE_PATH SHADERS "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/shaders" )
set( SHADERS "${SHADERS}/" )

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/config.hpp.in"
	"${PROJECT_BINARY_DIR}/MBConfig.hpp"
)

#find_package(Boost REQUIRED)
#TODO: download, build and install boost framework for iOS to ~/Documents automatically

include_directories("${PROJECT_BINARY_DIR}")

# Source files
set (SRC
	HelloIOS.cpp
)

# Headers
set (HDR
	${PROJECT_BINARY_DIR}/MBConfig.hpp HelloIOS.hpp
)

# Library
add_library (engine STATIC ${HDR} ${SRC})
source_group(\\ FILES ${HDR} ${SRC})

enable_testing()
add_subdirectory(test)

# Standard setup
cmake_minimum_required (VERSION 2.8)
project (engine CXX C)

#find_package(Boost REQUIRED)

# Includes
include_directories (${PROJECT_SOURCE_DIR})

# Source files
set (SRC
	HelloIOS.c
)

# Headers
set (HDR
	HelloIOS.h
)

# Library
add_library (hello-lib STATIC ${HDR} ${SRC})
source_group(\\ FILES ${HDR} ${SRC})

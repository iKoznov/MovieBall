# Standard setup
cmake_minimum_required (VERSION 2.8)
project (engine CXX C)

# Includes
include_directories (${hello-lib_SOURCE_DIR})

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

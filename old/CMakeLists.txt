cmake_minimum_required(VERSION 2.8)

project(SPHERE)

add_subdirectory( model )
add_subdirectory( testbed )

include_directories( model )
include_directories( testbed )


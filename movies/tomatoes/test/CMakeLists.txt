cmake_minimum_required(VERSION 2.8)

project(RED_BLACK_TREE)

#find_path(LOKI_INCLUDE_PATH loki)
#include_directories("${LOKI_INCLUDE_PATH}/")

set(SOURCE_MODEL
	RedBlack.c
    )

add_executable( tree ${SOURCE_MODEL} )

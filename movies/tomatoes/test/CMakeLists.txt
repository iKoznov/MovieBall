cmake_minimum_required(VERSION 2.8)

project(Pool)

find_path(LOKI_INCLUDE_PATH loki)
include_directories("${LOKI_INCLUDE_PATH}/")

set(SOURCE_MODEL
	pool.cpp
    )

add_executable( pool ${SOURCE_MODEL} )

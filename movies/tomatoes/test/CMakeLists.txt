cmake_minimum_required(VERSION 2.8)

project(Pool)

set(SOURCE_MODEL
	typetraits.hpp
    allocator.hpp
	AllocatorTest.cpp
    )

add_executable( pool ${SOURCE_MODEL} )

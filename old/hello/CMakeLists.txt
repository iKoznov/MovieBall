cmake_minimum_required(VERSION 2.8)

project(HELLO)

add_library( hello STATIC hello.h hello.c )
add_executable( main main.c )

target_link_libraries( main hello )

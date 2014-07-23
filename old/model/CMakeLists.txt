cmake_minimum_required(VERSION 2.8)

project(MODEL)
#add_subdirectory(src)

#find_package(OpenGL)
#find_package(GLUT)

#SET( CMAKE_C_COMPILER "c99wrap.exe ""cl""" )
#SET( CMAKE_CXX_COMPILER cl )

set(SOURCE_MODEL
    TargetConditionals.h
    model.h model.c
    )

#message( STATUS "==>> ${OPENGL_INCLUDE_DIR}" )
#message( STATUS "${GLUT_LIBRARY}" )

#include_directories( ${OPENGL_INCLUDE_DIR} )


#IF(APPLE)
#   INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
#   FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
#   MARK_AS_ADVANCED (OpenGL_LIBRARY)
#   SET(EXTRA_LIBS ${OpenGL_LIBRARY})
#ENDIF (APPLE)


#include_directories(/usr/X11R6/include/)
#link_directories(/usr/X11R6/lib)
#SET(EXTRA_LIBS GL X11 GLU glut)
#target_link_libraries(main ${EXTRA_LIBS})


#get_cmake_property(SOURCE MACROS)
#foreach (_var ${SOURCE})
#    message(STATUS "==========================>> ${SOURCE}")
#endforeach()

#add_custom_command(
#  OUTPUT icosahedron.cpp
#  COMMAND c99conv.exe icosahedron.c icosahedron.c
#  MAIN_DEPENDENCY icosahedron.c
#  COMMENT "Preprocessing myfile.sql.in"
#  VERBATIM
#)

add_library( model STATIC ${SOURCE_MODEL} )

#target_link_libraries( sphere_testbed ${EXTRA_LIBS} )
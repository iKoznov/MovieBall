cmake_minimum_required(VERSION 2.8)

project(SPHERE)
#add_subdirectory(src)

#find_package(OpenGL)
#find_package(GLUT)

set(SOURCE main.c)

#message( STATUS "==>> ${OPENGL_INCLUDE_DIR}" )
#message( STATUS "${GLUT_LIBRARY}" )

#include_directories( ${OPENGL_INCLUDE_DIR} )


IF(APPLE)
   INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(GLUT_LIBRARY GLUT )
   FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
   MARK_AS_ADVANCED (COCOA_LIBRARY
                     GLUT_LIBRARY
                     OpenGL_LIBRARY)
   SET(EXTRA_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
ENDIF (APPLE)


#include_directories(/usr/X11R6/include/)
#link_directories(/usr/X11R6/lib)
#SET(EXTRA_LIBS GL X11 GLU glut)
#target_link_libraries(main ${EXTRA_LIBS})


#get_cmake_property(SOURCE MACROS)
#foreach (_var ${SOURCE})
#    message(STATUS "==========================>> ${SOURCE}")
#endforeach()


add_executable( sphere ${SOURCE} )

target_link_libraries(sphere ${EXTRA_LIBS})



cmake_minimum_required(VERSION 2.8)

project(TESTBED)
#add_subdirectory(src)

#find_package(OpenGL)
#find_package(GLUT)

#SET( CMAKE_C_COMPILER "c99wrap.exe ""cl""" )
#SET( CMAKE_CXX_COMPILER cl )

set(SOURCE_TESTBED
    main.c
    )

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

IF(WIN32)
	INCLUDE_DIRECTORIES ( ../libsWIN32 )
	#SET( EXTRA_LIBS glut32 )
	link_directories("$../libsWIN32/GLUT")
	#SET( CMAKE_C_COMPILER c99conv.exe )
	#SET( CMAKE_CXX_COMPILER c99conv.exe )
ENDIF(WIN32)


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

# add_subdirectory( engine )
include_directories( ../model )

add_executable( testbed ${SOURCE_TESTBED} )

target_link_libraries( testbed ${EXTRA_LIBS} model )
#target_link_libraries( sphere_testbed ${EXTRA_LIBS} )

IF(WIN32)
	add_custom_command(TARGET sphere_testbed
	POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/GLUT/glut32.dll"      # <--this is in-file
        $<TARGET_FILE_DIR:sphere_testbed>)                 # <--this is out-file path
ENDIF(WIN32)
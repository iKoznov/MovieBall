/* engine.h */

#ifndef ENGINE_H
#define ENGINE_H

#if defined WIN32 | WIN64
#include <Windows.h>
#include <WinDef.h>
#define bool BOOL
#define true TRUE
#define false FALSE
#define __PRETTY_FUNCTION__ __FUNCSIG__
#else
#include <stdbool.h>
#endif

#include <stddef.h>
#include <math.h>

#if defined WIN32 | WIN64
#include <gl/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14
#endif

extern void _printMatrixf (GLfloat *m);

#endif

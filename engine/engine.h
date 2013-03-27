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

typedef union {
    struct { GLfloat v[3]; };
    struct { GLfloat x, y, z; };
    struct { GLfloat R, G, B; };
} vec3f;

typedef union {
    struct { GLfloat v[4]; };
    struct { GLfloat x, y, z, k; };
    struct { GLfloat R, G, B, A; };
} vec4f;

vec3f vec3fMake( GLfloat, GLfloat, GLfloat );
vec4f vec4fMake( GLfloat, GLfloat, GLfloat, GLfloat );

extern void _printMatrixf (GLfloat *m);

#endif

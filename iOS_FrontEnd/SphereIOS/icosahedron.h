//
//  icosahedron.h
//  SphereIOS
//
//  Created by user on 4/22/13.
//  Copyright (c) 2013 Biarum. All rights reserved.
//

#ifndef SphereIOS_icosahedron_h
#define SphereIOS_icosahedron_h

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

void setupDepthBuffer();
void setupRenderBuffer();
void setupFrameBuffer();

void compileShaders();
void setupVBOs();
void render();

/* Common parametres */
GLsizei _width;
GLsizei _height;
GLchar *_resoucePath;

GLuint _colorRenderBuffer;
GLuint _depthRenderBuffer;

/* Shader Attribs */
GLuint _positionSlot;
GLuint _colorSlot;
GLuint _texCoordSlot;
GLuint _textureUniform;

/* Shader Const */
GLuint _projectionUniform;
GLuint _modelViewUniform;
GLuint _SurfaceCenterUniform;

/* Scene params */
float _currentRotation;
GLfloat _lon;
GLfloat _lat;

#endif

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
#import <GLKit/GLKit.h>
#include "tomatoes.h"

void setupDepthBuffer();
void setupRenderBuffer();
void setupFrameBuffer();

void compileShaders();
void setupVBOs();
void render();

void loadPosters();

/* Common parametres */
GLsizei _width;
GLsizei _height;
GLchar *_resoucePath;
GLuint _defaultTexture;
Movie _rootMovie;

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
GLKMatrix4 _rotMatrix;
GLfloat _lon;
GLfloat _lat;

#endif

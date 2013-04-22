//
//  icosahedron.c
//  SphereIOS
//
//  Created by user on 4/22/13.
//  Copyright (c) 2013 Biarum. All rights reserved.
//


#include <CoreGraphics/CoreGraphics.h>
#import <GLKit/GLKit.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "icosahedron.h"
#include "mStr.h"

#pragma mark -
#pragma mark Render and frame buffer setup

void setupDepthBuffer()
{
    glGenRenderbuffers(1, &_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _width, _height);
}

void setupRenderBuffer()
{
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
}

void setupFrameBuffer()
{
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
}

#pragma mark -
#pragma mark Shaders

GLuint compileShader(const char *shaderName, const GLenum shaderType)
{
    mStr nameBuff = mStr_make( strdup(_resoucePath) );
    mStr_addStr( nameBuff, strdup("/") );
    mStr_addStr( nameBuff, strdup(shaderName) );
    mStr_addStr( nameBuff, strdup(".glsl") );
    char *fName = mStr_string( nameBuff );
    mStr_free(nameBuff);
    
    FILE *fp = fopen( fName, "r" );
    free(fName);
    if(fp==NULL) perror("Error loading shader");
    fseek( fp, 0, SEEK_END );
    int numBytes = ftell( fp ); // length of file
    GLchar * buffer = malloc(numBytes);
    rewind( fp ); // same as: “fseek( in, 0, SEEK_SET )”
    fread( buffer, 1, numBytes, fp );
    fclose( fp );
    
    GLuint shaderHandle = glCreateShader(shaderType);
    
    glShaderSource( shaderHandle, 1, (const GLchar **)&buffer, &numBytes );
    free(buffer);
    fclose(fp);
    
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        perror(messages);
        exit(1);
    }
    
    return shaderHandle;
}

void compileShaders()
{
    GLuint vertexShader = compileShader("SimpleVertex", GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader("SimpleFragment", GL_FRAGMENT_SHADER);
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        perror(messages);
        exit(1);
    }
    
    glUseProgram(programHandle);
    
    _positionSlot = glGetAttribLocation(programHandle, "Position");
    _colorSlot = glGetAttribLocation(programHandle, "SourceColor");
    glEnableVertexAttribArray(_positionSlot);
    glEnableVertexAttribArray(_colorSlot);
    
    _projectionUniform = glGetUniformLocation(programHandle, "Projection");
    _modelViewUniform = glGetUniformLocation(programHandle, "Modelview");
    _SurfaceCenterUniform = glGetUniformLocation(programHandle, "SurfaceCenter");
    
    _texCoordSlot = glGetAttribLocation(programHandle, "TexCoordIn");
    glEnableVertexAttribArray(_texCoordSlot);
    _textureUniform = glGetUniformLocation(programHandle, "Texture");
}

#pragma mark -
#pragma mark Textures
GLuint setupTexture(NSString *fileName)
{
    CGImageRef spriteImage = [UIImage imageNamed:fileName].CGImage;
    if (!spriteImage) {
        NSLog(@"Failed to load image %@", fileName);
        exit(1);
    }
    
    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);
    
    GLubyte * spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    CGContextRelease(spriteContext);
    
    GLuint texName;
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    free(spriteData);
    return texName;
}

#pragma mark -
#pragma mark Vertex data
typedef union {
    struct {
        GLfloat Position[3];
        GLfloat Color[4];
        GLfloat TexCoord[2];
    };
    struct {
        GLfloat x, y, z;
        GLfloat R, G, B, A;
        GLfloat tX, tY;
    };
} Vertex;

void _printMatrixf (GLfloat *m) {
    printf("---------------------------\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n", m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14], m[3], m[7], m[11], m[15]);
}

GLuint _n, _N;
Vertex *_vertices;
void icosahedron_initialize(GLuint n)
{
	GLfloat R, a, h, R5;
    GLfloat hLat, sLon, lat, lon;
	GLint i, j;
    Vertex *p;
    
    R = 1.8f;
    _n = n;
    _N = 2 + 10 * _n*_n;
    a = 4.0f * R / sqrtf(10.0f + 2.0f*sqrtf(5.0f));
    h = a * sqrtf( (5.0f - sqrtf(5.0f)) / 10.0f );
    R5 = a * sqrtf( (5.0f + sqrtf(5.0f)) / 10.0f );
    hLat = asinf( (R-h) / R);
    
    printf("%s\n", __PRETTY_FUNCTION__);
    printf("\tR = %f\n", R);
    printf("\ta = %f\n", a);
    printf("\th = %f\n", h);
    printf("\tR5 = %f\n", R5);
    printf("\thLat = %f\n", hLat);
    
    _vertices = p = calloc( _N, sizeof(Vertex) );
    
    (*p).x = 0;
    (*p).y = R;
    (*p).z = 0;
    p++;
    
    for (j = 1; j < _n + 1; j++)
    {
        lat = hLat + (_n-j) * ((GLfloat)M_PI/2.0f - hLat) / _n;
        sLon = 0.0f;
        for (i = 0; i < 5*(j); p++, i++)
        {
            lon = sLon + i*2.0f*(GLfloat)M_PI/(5*j);
            (*p).x = R * cosf(lat) * cosf(lon);
            (*p).y = R * sinf(lat);
            (*p).z = R * cosf(lat) * sinf(lon);
        }
    }
    
    for (j = 1; j < _n; j++)
    {
        lat = hLat * (1.0f - j*2.0f/_n);
        sLon = (GLfloat)j/(GLfloat)_n * (GLfloat)M_PI/5.0f;
        printf("sLon : %f\n", sLon);
        for (i = 0; i < 5*_n; p++, i++)
        {
            GLfloat lon = sLon + i*2.0f*(GLfloat)M_PI/(5*_n);
            (*p).x = R * cosf(lat) * cosf(lon);
            (*p).y = R * sinf(lat);
            (*p).z = R * cosf(lat) * sinf(lon);
        }
    }
    
    for (j = 1; j < _n + 1; j++)
    {
        lat = -hLat - (_n-j) * ((GLfloat)M_PI/2.0f - hLat) / _n;
        sLon = (GLfloat)M_PI/5.0f;
        for (i = 0; i < 5*(j); p++, i++)
        {
            lon = sLon + i*2.0f*(GLfloat)M_PI/(5*j);
            (*p).x = R * cosf(lat) * cosf(lon);
            (*p).y = R * sinf(lat);
            (*p).z = R * cosf(lat) * sinf(lon);
        }
    }
    
    (*p).x = 0;
    (*p).y = -R;
    (*p).z = 0;
}

typedef struct {
    GLfloat center[3];
    GLuint *indexes;
    GLuint numberOfVertices;
    GLuint indexBuffer;
    GLuint texture;
} Side;



GLuint _vertexBuffer;
GLuint _indexBuffer;
Side *_sides;
GLuint _numberOfSides;
void icosahedron_indexes()
{
    GLuint n = 1;
    icosahedron_initialize(3*n);
    
    _numberOfSides = 0;
    Side *p = _sides = calloc(12+20, sizeof(Side));
    
#define SIDE(...) { \
GLuint s[] = {__VA_ARGS__}; \
p->indexes = malloc(sizeof(s)); \
memcpy(p->indexes, s, sizeof(s)); \
p->numberOfVertices = sizeof(s)/sizeof(s[0]); \
for (GLuint i = 0; i < p->numberOfVertices; i++) \
{ \
p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices; \
p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices; \
p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices; \
\
_vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;\
_vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;\
}\
\
glGenBuffers(1, &(p->indexBuffer));\
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);\
glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);\
\
p->texture = setupTexture(@"american-beauty0021.png");\
\
_numberOfSides++;\
p++;\
}
    
#pragma mark TOP
    SIDE(1,2,3,4,5)
    
#pragma mark SECOND ROW
    SIDE(1,2,8,18,17,6)
    SIDE(2,3,10,21,20,8)
    SIDE(3,4,12,24,23,10)
    SIDE(4,5,14,27,26,12)
    SIDE(5,1,6,30,29,14)
    
#pragma mark TOP CENTER 5
    SIDE(6,17,31,45,30)
    SIDE(8,20,34,33,18)
    SIDE(10,23,37,36,21)
    SIDE(12,26,40,39,24)
    SIDE(14,29,43,42,27)
    
#pragma mark TOP CENTER 6
    SIDE(17,18,33,47,46,31)
    SIDE(20,21,36,50,49,34)
    SIDE(23,24,39,53,52,37)
    SIDE(26,27,42,56,55,40)
    SIDE(29,30,45,59,58,43)
    
#pragma mark BOTTOM CENTER 6
    //    SIDE(33,34,49,63,62,47)
    
#pragma mark BOTTOM
    SIDE(61,62,63,64,65)
    
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _N * sizeof(Vertex), _vertices, GL_STATIC_DRAW);
    
}

void setupVBOs()
{
    icosahedron_indexes();
}

void render()
{
    glClearColor(255.0/255.0, 255.0/255.0, 255.0/255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    GLfloat h = 4.0f * _height / _width;
    glUniformMatrix4fv( _projectionUniform, 1, 0, GLKMatrix4MakeFrustum( -1, 1, -h/4, h/4, 2, 10 ).m );
    
    GLKMatrix4 modelView = GLKMatrix4MakeTranslation( 0, 0, -4 );
    modelView = GLKMatrix4RotateX(modelView, _lat);
    modelView = GLKMatrix4RotateY(modelView, _lon);
    glUniformMatrix4fv(_modelViewUniform, 1, 0, modelView.m);
    
    glViewport(0, 0, _width, _height);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    for (GLuint i=0; i<_numberOfSides; i++)
    {
        glUniform3f(_SurfaceCenterUniform, _sides[i].center[0], _sides[i].center[1], _sides[i].center[2]);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _sides[i].indexBuffer);
        glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Position));
        glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Color));
        glVertexAttribPointer(_texCoordSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoord));
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _sides[i].texture);
        glUniform1i(_textureUniform, 0);
        
        glDrawElements(GL_TRIANGLE_FAN, _sides[i].numberOfVertices, GL_UNSIGNED_INT, 0);
    }
}

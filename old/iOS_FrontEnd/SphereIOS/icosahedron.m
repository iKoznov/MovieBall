//
//  icosahedron.c
//  SphereIOS
//
//  Created by user on 4/22/13.
//  Copyright (c) 2013 Biarum. All rights reserved.
//


#include <CoreGraphics/CoreGraphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "icosahedron.h"
#include "mStr.h"
#include "RedBlack.h"

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

void _printMatrixf (const GLfloat *const m) {
    printf("---------------------------\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n", m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14], m[3], m[7], m[11], m[15]);
}

GLuint _n, _N;
Vertex *_vertices;
void icosahedron_initialize(GLuint n)
{
	GLfloat R, a, h, R5;
	GLint i, j, k;
    Vertex *p;
    
    R = 1.2f;
    _n = n;
    _N = 2 + 10 * _n*_n;
    a = 4.0f * R / sqrtf(10.0f + 2.0f*sqrtf(5.0f));
    h = a * sqrtf( (5.0f - sqrtf(5.0f)) / 10.0f );
    R5 = a * sqrtf( (5.0f + sqrtf(5.0f)) / 10.0f );
    
    printf("%s\n", __PRETTY_FUNCTION__);
    printf("\tR = %f\n", R);
    printf("\ta = %f\n", a);
    printf("\th = %f\n", h);
    printf("\tR5 = %f\n", R5);
    
    _vertices = p = calloc( _N, sizeof(Vertex) );
    
    (*p).x = 0;
    (*p).y = R;
    (*p).z = 0;
    p++;
    
    for (j = 1; j < _n + 1; j++)
    {
        GLfloat Y = R - j * h / _n;
        for (i = 0; i < 5; i++)
        {
            GLfloat x0 = (R5*j/_n) * cosf((GLfloat)i*0.4f*(GLfloat)M_PI);
            GLfloat z0 = (R5*j/_n) * sinf((GLfloat)i*0.4f*(GLfloat)M_PI);
            GLfloat x1 = (R5*j/_n) * cosf((GLfloat)(i+1)*0.4f*(GLfloat)M_PI);
            GLfloat z1 = (R5*j/_n) * sinf((GLfloat)(i+1)*0.4f*(GLfloat)M_PI);
            for (k = 0; k < j; k++)
            {
                (*p).x = x0 + k * (x1 - x0) / j;
                (*p).y = Y;
                (*p).z = z0 + k * (z1 - z0) / j;
                p++;
            }
        }
    }
    
    for (j = 1; j < _n; j++)
    {
        GLfloat Y = (R-h) - j * (2*R - 2*h) / _n;
        for (i = 0; i < 5; i++)
        {
            GLfloat ax0 = R5 * cosf((GLfloat)i*0.4f*(GLfloat)M_PI);
            GLfloat az0 = R5 * sinf((GLfloat)i*0.4f*(GLfloat)M_PI);
            GLfloat ax1 = R5 * cosf((GLfloat)(i+1)*0.4f*(GLfloat)M_PI);
            GLfloat az1 = R5 * sinf((GLfloat)(i+1)*0.4f*(GLfloat)M_PI);
            GLfloat bx0 = R5 * cosf((GLfloat)(i+0.5f)*0.4f*(GLfloat)M_PI);
            GLfloat bz0 = R5 * sinf((GLfloat)(i+0.5f)*0.4f*(GLfloat)M_PI);
            GLfloat bx1 = R5 * cosf((GLfloat)(i+1.5f)*0.4f*(GLfloat)M_PI);
            GLfloat bz1 = R5 * sinf((GLfloat)(i+1.5f)*0.4f*(GLfloat)M_PI);
            
            GLfloat cx0 = ax0 + j * (bx0 - ax0) / _n;
            GLfloat cz0 = az0 + j * (bz0 - az0) / _n;
            GLfloat cx1 = ax1 + j * (bx0 - ax1) / _n;
            GLfloat cz1 = az1 + j * (bz0 - az1) / _n;
            GLfloat cx2 = ax1 + j * (bx1 - ax1) / _n;
            GLfloat cz2 = az1 + j * (bz1 - az1) / _n;
            
            for (k = 0; k < (_n-j); k++)
            {
                (*p).x = cx0 + k * (cx1 - cx0) / (_n-j);
                (*p).y = Y;
                (*p).z = cz0 + k * (cz1 - cz0) / (_n-j);
                p++;
            }
            
            for (k = 0; k < j; k++)
            {
                (*p).x = cx1 + k * (cx2 - cx1) / j;
                (*p).y = Y;
                (*p).z = cz1 + k * (cz2 - cz1) / j;
                p++;
            }
        }
    }
    
    for (j = 1; j < _n + 1; j++)
    {
        GLfloat Y = (h-R) - (j-1) * h / _n;
        for (i = 0; i < 5; i++)
        {
            GLfloat x0 = (R5*(_n-j+1)/_n) * cosf((GLfloat)(i+0.5f)*0.4f*(GLfloat)M_PI);
            GLfloat z0 = (R5*(_n-j+1)/_n) * sinf((GLfloat)(i+0.5f)*0.4f*(GLfloat)M_PI);
            GLfloat x1 = (R5*(_n-j+1)/_n) * cosf((GLfloat)(i+1.5f)*0.4f*(GLfloat)M_PI);
            GLfloat z1 = (R5*(_n-j+1)/_n) * sinf((GLfloat)(i+1.5f)*0.4f*(GLfloat)M_PI);
            for (k = 0; k < (_n-j+1); k++)
            {
                (*p).x = x0 + k * (x1 - x0) / (_n-j+1);
                (*p).y = Y;
                (*p).z = z0 + k * (z1 - z0) / (_n-j+1);
                p++;
            }
        }
    }
    
    (*p).x = 0;
    (*p).y = -R;
    (*p).z = 0;
}

typedef struct _Polygon Polygon;
struct _Polygon
{
    GLfloat center[3];
    GLuint numberOfVertices;
    GLuint indexBuffer;
    GLuint texture;
    
    GLuint *neigbours;
    GLuint numberOfNeigbours;
    
    Movie movie;
};

GLuint _vertexBuffer;
GLuint _indexBuffer;
struct _Polygon *_sides;
GLuint _numberOfPolygones;
void icosahedron_indexes()
{
    GLuint n = 1;
    icosahedron_initialize(3*n);
    
    _numberOfPolygones = 0;
    struct _Polygon *p = _sides = calloc(12+20, sizeof(struct _Polygon));
    
#define SIDE(...) { \
    GLuint s[] = {__VA_ARGS__}; \
    GLuint numberOfVertices = p->numberOfVertices = sizeof(s)/sizeof(s[0]); \
    for (GLuint i = 0; i < numberOfVertices; i++) \
    { \
        p->center[0] += _vertices[s[i]].x / (GLfloat)numberOfVertices; \
        p->center[1] += _vertices[s[i]].y / (GLfloat)numberOfVertices; \
        p->center[2] += _vertices[s[i]].z / (GLfloat)numberOfVertices; \
        \
        _vertices[s[i]].tX = 0.5f + cosf(i*2.0f*M_PI/(GLfloat)numberOfVertices)/2.0f;\
        _vertices[s[i]].tY = 0.5f + sinf(i*2.0f*M_PI/(GLfloat)numberOfVertices)/2.0f;\
    }\
    \
    glGenBuffers(1, &(p->indexBuffer));\
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);\
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfVertices * sizeof(s[0]), s, GL_STATIC_DRAW);\
    \
    p->texture = _defaultTexture;\
    \
    _numberOfPolygones++;\
    p++;\
}
    
#define NEIGBOURS( index, ... ) { \
    GLuint neigbours[] = {__VA_ARGS__}; \
    GLuint numberOfNeigbours = sizeof(neigbours) / sizeof(GLuint); \
    _sides[index].neigbours = calloc(numberOfNeigbours, sizeof(GLuint)); \
    _sides[index].numberOfNeigbours = numberOfNeigbours; \
    memcpy(_sides[index].neigbours, neigbours, sizeof(neigbours)); \
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
    SIDE(86,87,88,89,90)
    
    NEIGBOURS(0, 1,2,3,4,5)
    NEIGBOURS(1, 0)
    NEIGBOURS(2, 0)
    NEIGBOURS(3, 0)
    NEIGBOURS(4, 0)
    NEIGBOURS(5, 0)
    
//    _rootMovie
    
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _N * sizeof(Vertex), _vertices, GL_STATIC_DRAW);
}

void setupVBOs()
{
    _defaultTexture = setupTexture(@"american-beauty0021.png");
    
    icosahedron_indexes();
    _rotMatrix = GLKMatrix4Identity;
}

GLuint loadPosterTexture(char *url)
{
    NSString *urlString = [NSString stringWithUTF8String:url];
    UIImage *image = [UIImage imageWithData:[NSData dataWithContentsOfURL:[NSURL URLWithString:urlString]]];
    
    CGImageRef spriteImage = image.CGImage;
    if (!spriteImage) {
        NSLog(@"Failed to load image %s", url);
        exit(1);
    }
    
    size_t width = 1024;//CGImageGetWidth(spriteImage);
    size_t height = 1024;//CGImageGetHeight(spriteImage);
    float ratio = (float)CGImageGetWidth(spriteImage) / (float)CGImageGetHeight(spriteImage);
    
    NSLog(@"TEXTURE : %zd x %zd", width, height);
    
    GLubyte * spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    CGContextDrawImage(spriteContext, CGRectMake( 0, (height - height/ratio) / 2.0f, width, height/ratio), spriteImage);
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

typedef struct _Priority Priority;
struct _Priority {
    MovieNode node;
    float value;
};

redBlackType(Priority)

#define compLT(a,b) ( a.value > b.value )
#define compEQ(a,b) ( a.value == b.value )
redBlack(Priority)

void loadPosters()
{
    _sides[0].movie = _rootMovie;
    _sides[0].texture = loadPosterTexture(_sides[0].movie->original);
    
    MovieNode node = NodeByMovie(_rootMovie);
    firstNodeMovieNode(getNODES());
    
    SetPriority priority = SetMakePriority();
    
    NodeMovieNodePointer *lnk = firstNodeMovieNodePointer( &node->links );
    while ( lnk ) {
        NodePair *pair = PairByNodes( node, nodeDataMovieNodePointer(lnk) );
        Priority current;
        current.node = nodeDataMovieNodePointer(lnk);
        current.value = Force( pair );
        insertNodePriority( &priority, current );
        lnk = nextNodeMovieNodePointer( lnk );
    }
    
    NodePriority *p = firstNodePriority( &priority );
    int i = 0;
    NSLog(@"numberOfNeigbours : %d", _sides[0].numberOfNeigbours);
    while (p && i < _sides[0].numberOfNeigbours) {
        GLuint neigbour = _sides[0].neigbours[i];
        _sides[neigbour].movie = nodeDataPriority(p).node->movie;
        _sides[neigbour].texture = loadPosterTexture(_sides[neigbour].movie->original);
        p = nextNodePriority( p );
        i++;
    }
}

GLfloat lastX = 0;
GLfloat lastY = 0;
void render()
{
    glClearColor(255.0/255.0, 255.0/255.0, 255.0/255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    GLfloat h = 4.0f * _height / _width;
    glUniformMatrix4fv( _projectionUniform, 1, 0, GLKMatrix4MakeOrtho( -1, 1, -h/4, h/4, 2, 10 ).m );
//    glUniformMatrix4fv( _projectionUniform, 1, 0, GLKMatrix4Identity.m );
//    _printMatrixf( GLKMatrix4Identity.m );
    
    GLfloat rotX = (_lat - lastX) / 200.0f;
    GLfloat rotY = (_lon - lastY) / 200.0f;
    lastX = _lat;
    lastY = _lon;
    
    bool isInvertible;
    GLKVector3 xAxis = GLKMatrix4MultiplyVector3(GLKMatrix4Invert(_rotMatrix, &isInvertible), GLKVector3Make(1, 0, 0));
    _rotMatrix = GLKMatrix4Rotate(_rotMatrix, rotX, xAxis.x, xAxis.y, xAxis.z);
    GLKVector3 yAxis = GLKMatrix4MultiplyVector3(GLKMatrix4Invert(_rotMatrix, &isInvertible), GLKVector3Make(0, 1, 0));
    _rotMatrix = GLKMatrix4Rotate(_rotMatrix, rotY, yAxis.x, yAxis.y, yAxis.z);
    
    GLKMatrix4 modelView = GLKMatrix4MakeTranslation( 0, 0, -4 );
    modelView = GLKMatrix4Multiply(modelView, _rotMatrix);
    glUniformMatrix4fv(_modelViewUniform, 1, 0, modelView.m);
    
    glViewport(0, 0, _width, _height);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    for (GLuint i=0; i<_numberOfPolygones; i++)
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

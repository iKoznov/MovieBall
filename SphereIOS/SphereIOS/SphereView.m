//
//  SphereView.m
//  SphereIOS
//
//  Created by IGOR KOZNOV on 28.03.13.
//  Copyright (c) 2013 Biarum. All rights reserved.
//

#import "SphereView.h"
#import "CC3GLMatrix.h"


#pragma mark -
#pragma mark SphereView implementation
@implementation SphereView

#pragma mark -
#pragma mark Initialization
- (id)init { self = [super init]; [self customInit]; return self; }
- (id)initWithFrame:(CGRect)frame { self = [super initWithFrame:frame]; [self customInit]; return self; }
- (void)customInit {
    if (_didCustomInit) return;
    _didCustomInit = YES;
    [self setupLayer];
    [self setupContext];
    [self setupDepthBuffer];
    [self setupRenderBuffer];
    [self setupFrameBuffer];
    [self compileShaders];
    [self setupVBOs];
    [self setupDisplayLink];
}

#pragma mark -
#pragma mark Layer setup
+ (Class)layerClass { return [CAEAGLLayer class]; }
- (void)setupLayer { _eaglLayer = (CAEAGLLayer *)self.layer; _eaglLayer.opaque = YES; _eaglLayer.contentsScale = [[UIScreen mainScreen] scale]; }
- (void)setupDisplayLink {
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

#pragma mark Context setup
- (void)setupContext {
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    _context = [[EAGLContext alloc] initWithAPI:api];
    if (!_context) { NSLog(@"Failed to initialize OpenGLES 2.0 context"); exit(1); }
    if (![EAGLContext setCurrentContext:_context]) { NSLog(@"Failed to set current OpenGL context"); exit(1); }
}

#pragma mark -
#pragma mark Render and frame buffer setup
- (void)setupDepthBuffer {
    glGenRenderbuffers(1, &_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, self.frame.size.width*self.layer.contentsScale, self.frame.size.height*self.layer.contentsScale);
}
- (void)setupRenderBuffer {
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
}
- (void)setupFrameBuffer {
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
}

#pragma mark -
#pragma mark Shaders
- (GLuint)compileShader:(NSString*)shaderName withType:(GLenum)shaderType {
    NSError* error;
    NSString* shaderPath = [[NSBundle mainBundle] pathForResource:shaderName ofType:@"glsl"];
    NSString* shaderString = [NSString stringWithContentsOfFile:shaderPath encoding:NSUTF8StringEncoding error:&error];
    if (!shaderString) { NSLog(@"Error loading shader: %@", error.localizedDescription); exit(1); }
    
    GLuint shaderHandle = glCreateShader(shaderType);
    
    const char * shaderStringUTF8 = [shaderString UTF8String];
    int shaderStringLength = [shaderString length];
    glShaderSource(shaderHandle, 1, &shaderStringUTF8, &shaderStringLength);
    
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"%@", messageString);
        exit(1);
    }
    
    return shaderHandle;
}

- (void)compileShaders {
    GLuint vertexShader = [self compileShader:@"SimpleVertex" withType:GL_VERTEX_SHADER];
    GLuint fragmentShader = [self compileShader:@"SimpleFragment" withType:GL_FRAGMENT_SHADER];
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"%@", messageString);
        exit(1);
    }
    
    glUseProgram(programHandle);
    
    _positionSlot = glGetAttribLocation(programHandle, "Position");
    _colorSlot = glGetAttribLocation(programHandle, "SourceColor");
    glEnableVertexAttribArray(_positionSlot);
    glEnableVertexAttribArray(_colorSlot);
    
    _projectionUniform = glGetUniformLocation(programHandle, "Projection");
    _modelViewUniform = glGetUniformLocation(programHandle, "Modelview");
    
//    _texCoordSlot = glGetAttribLocation(programHandle, "TexCoordIn");
//    glEnableVertexAttribArray(_texCoordSlot);
//    _textureUniform = glGetUniformLocation(programHandle, "Texture");
    
}

#pragma mark -
#pragma mark Vertex data
typedef union {
    struct {
        GLfloat Position[3];
        GLfloat Color[4];
    };
    struct {
        GLfloat x, y, z;
        GLfloat R, G, B, A;
    };
} Vertex;

GLuint _n, _N;
Vertex *_vertices;
void icosahedron_initialize()
{
	GLfloat R, a, h, R5;
    GLfloat hLat, sLon, lat, lon;
	GLint i, j;
    Vertex *p;
    
    R = 1.8f;
    _n = 20;
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

- (void)setupVBOs {
    
    icosahedron_initialize();
    
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _N * sizeof(Vertex), _vertices, GL_STATIC_DRAW);
    
//    GLuint indexBuffer;
//    glGenBuffers(1, &indexBuffer);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

- (void)render:(CADisplayLink*)displayLink {
    glClearColor(0, 104.0/255.0, 55.0/255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    CC3GLMatrix *projection = [CC3GLMatrix matrix];
    float h = 4.0f * self.frame.size.height / self.frame.size.width;
    [projection populateFromFrustumLeft:-2 andRight:2 andBottom:-h/2 andTop:h/2 andNear:4 andFar:10];
    glUniformMatrix4fv(_projectionUniform, 1, 0, projection.glMatrix);
    
    CC3GLMatrix *modelView = [CC3GLMatrix matrix];
    [modelView populateFromTranslation:CC3VectorMake(sin(CACurrentMediaTime()), 0, -7)];
    _currentRotation += displayLink.duration * 10;
    [modelView rotateBy:CC3VectorMake(_currentRotation, _currentRotation, 0)];
    glUniformMatrix4fv(_modelViewUniform, 1, 0, modelView.glMatrix);
    
    glViewport(0, 0, self.frame.size.width*self.layer.contentsScale, self.frame.size.height*self.layer.contentsScale);
    
    glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
    
    glDrawArrays(GL_TRIANGLES, 0, _N);
    //    glDrawElements(GL_POINTS, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_BYTE, 0);
    
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

@end

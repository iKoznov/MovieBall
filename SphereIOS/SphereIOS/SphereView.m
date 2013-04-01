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
    [self setupGestures];
    
    _floorTexture = setupTexture(@"tile_floor.png");
    _fishTexture = setupTexture(@"item_powerup_fish.png");
}

#pragma mark -
#pragma mark Gesture setup
- (void)setupGestures {
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panHandler:)];
    [self addGestureRecognizer:pan];
}
- (void)panHandler:(UIPanGestureRecognizer *)gesture {
    CGPoint translation = [gesture translationInView:self];
    _lon += translation.x;
    _lat += translation.y;
    [gesture setTranslation:CGPointZero inView:self];
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
    (*p).z = R;
    (*p).y = 0;
    p++;
    
    for (j = 1; j < _n + 1; j++)
    {
        lat = hLat + (_n-j) * ((GLfloat)M_PI/2.0f - hLat) / _n;
        sLon = 0.0f;
        for (i = 0; i < 5*(j); p++, i++)
        {
            lon = sLon + i*2.0f*(GLfloat)M_PI/(5*j);
            (*p).x = R * cosf(lat) * cosf(lon);
            (*p).z = R * sinf(lat);
            (*p).y = R * cosf(lat) * sinf(lon);
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
            (*p).z = R * sinf(lat);
            (*p).y = R * cosf(lat) * sinf(lon);
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
            (*p).z = R * sinf(lat);
            (*p).y = R * cosf(lat) * sinf(lon);
        }
    }
    
    (*p).x = 0;
    (*p).z = -R;
    (*p).y = 0;
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
    
#pragma mark TOP
    {
        GLuint s[] = {1,2,3,4,5};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
#pragma mark SECOND ROW
    {
        GLuint s[] = {1,2,8,18,17,6};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {2,3,10,21,20,8};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {3,4,12,24,23,10};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {4,5,14,27,26,12};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {5,1,6,30,29,14};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
#pragma mark TOP CENTER 5
    {
        GLuint s[] = {8,20,34,33,18};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {10,23,37,36,21};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {12,26,40,39,24};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {14,29,43,42,27};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {6,17,31,45,30};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
#pragma mark TOP CENTER 6
    {
        GLuint s[] = {17,18,33,47,46,31};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {20,21,36,50,49,34};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {23,24,39,53,52,37};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {26,27,42,56,55,40};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    {
        GLuint s[] = {29,30,45,59,58,43};
        p->indexes = malloc(sizeof(s));
        memcpy(p->indexes, s, sizeof(s));
        p->numberOfVertices = sizeof(s)/sizeof(s[0]);
        for (GLuint i = 0; i < p->numberOfVertices; i++)
        {
            p->center[0] += _vertices[p->indexes[i]].x / p->numberOfVertices;
            p->center[1] += _vertices[p->indexes[i]].y / p->numberOfVertices;
            p->center[2] += _vertices[p->indexes[i]].z / p->numberOfVertices;
            
            _vertices[p->indexes[i]].tX = 0.5f + cosf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            _vertices[p->indexes[i]].tY = 0.5f + sinf(i*2.0f*M_PI/p->numberOfVertices)/2.0f;
            NSLog(@"cord[%d] : %f %f %f", p->indexes[i], _vertices[p->indexes[i]].x, _vertices[p->indexes[i]].y, _vertices[p->indexes[i]].z );
            NSLog(@"texCord[%d] : %f %f", p->indexes[i], _vertices[p->indexes[i]].tX, _vertices[p->indexes[i]].tY );
        }
        
        glGenBuffers(1, &(p->indexBuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p->numberOfVertices * sizeof(p->indexes[0]), p->indexes, GL_STATIC_DRAW);
        
        p->texture = setupTexture(@"american-beauty0021.png");
        NSLog(@"center : %f %f %f", p->center[0], p->center[1], p->center[2]);
        
        _numberOfSides++;
        p++;
    }
    
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _N * sizeof(Vertex), _vertices, GL_STATIC_DRAW);
    
}

typedef struct {
    float Position[3];
    float Color[4];
    float TexCoord[2]; // New
} Vertex3;

#define TEX_COORD_MAX   1

const Vertex3 Vertices[] = {
    // Front
    {{1, -1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
    {{1, 1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{-1, 1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
    {{-1, -1, 0}, {0, 0, 0, 1}, {0, 0}},
    // Back
    {{1, 1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
    {{-1, -1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{1, -1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
    {{-1, 1, -2}, {0, 0, 0, 1}, {0, 0}},
    // Left
    {{-1, -1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
    {{-1, 1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{-1, 1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
    {{-1, -1, -2}, {0, 0, 0, 1}, {0, 0}},
    // Right
    {{1, -1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
    {{1, 1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{1, 1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
    {{1, -1, 0}, {0, 0, 0, 1}, {0, 0}},
    // Top
    {{1, 1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
    {{1, 1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{-1, 1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
    {{-1, 1, 0}, {0, 0, 0, 1}, {0, 0}},
    // Bottom
    {{1, -1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
    {{1, -1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{-1, -1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
    {{-1, -1, -2}, {0, 0, 0, 1}, {0, 0}}
};

const GLubyte Indices[] = {
    // Front
    0, 1, 2,
    2, 3, 0,
    // Back
    4, 5, 6,
    6, 7, 4,
    // Left
    8, 9, 10,
    10, 11, 8,
    // Right
    12, 13, 14,
    14, 15, 12,
    // Top
    16, 17, 18,
    18, 19, 16,
    // Bottom
    20, 21, 22,
    22, 23, 20
};

GLuint _vertexBuffer3;
GLuint _indexBuffer3;
- (void)setupVBOs {
    
    icosahedron_indexes();
    
    glGenBuffers(1, &_vertexBuffer3);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &_indexBuffer3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    
//    GLuint indexBuffer;
//    glGenBuffers(1, &indexBuffer);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    
//    glGenBuffers(1, &_vertexBuffer2);
//    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer2);
//    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), _vertices2, GL_STATIC_DRAW);
}

- (void)render:(CADisplayLink*)displayLink
{
//    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_BLEND);
    
    glClearColor(255.0/255.0, 255.0/255.0, 255.0/255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    CC3GLMatrix *projection = [CC3GLMatrix matrix];
    float h = 4.0f * self.frame.size.height / self.frame.size.width;
    [projection populateFromFrustumLeft:-1 andRight:1 andBottom:-h/4 andTop:h/4 andNear:2 andFar:10];
    glUniformMatrix4fv(_projectionUniform, 1, 0, projection.glMatrix);
    
    CC3GLMatrix *modelView = [CC3GLMatrix matrix];
    [modelView populateFromTranslation:CC3VectorMake(0, 0, -4)];
    [modelView rotateBy:CC3VectorMake(_lat, _lon, 0)];
    glUniformMatrix4fv(_modelViewUniform, 1, 0, modelView.glMatrix);
    
//    _printMatrixf(modelView.glMatrix);
    
    glViewport(0, 0, self.frame.size.width*self.layer.contentsScale, self.frame.size.height*self.layer.contentsScale);
    
    
//    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer3);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer3);
//    
//    // 2
//    glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//    glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
//    
//    glVertexAttribPointer(_texCoordSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 7));
//    
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, _floorTexture);
//    glUniform1i(_textureUniform, 0);
//    
//    // 3
//    glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_BYTE, 0);
//    
//    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
//    glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//    glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
//    glDrawArrays(GL_TRIANGLES, 0, _N);
    
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
        
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, _floorTexture);
//        glUniform1i(_textureUniform, 0);
        
        glDrawElements(GL_TRIANGLE_FAN, _sides[i].numberOfVertices, GL_UNSIGNED_INT, 0);
    }
    
//    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer2);
//    glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//    glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
//    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

@end

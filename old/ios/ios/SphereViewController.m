//
//  SphereViewController.m
//  ios
//
//  Created by user on 3/28/13.
//  Copyright (c) 2013 Biarum. All rights reserved.
//

#import "SphereViewController.h"
//#include "engine.h"
//#include "icosahedron.h"
//#include "xyz.h"

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

vec3f vec3fMake( GLfloat a, GLfloat b, GLfloat c ) {
    vec3f res;
    res.R = a;
    res.G = b;
    res.B = c;
    return res;
}

vec4f vec4fMake( GLfloat a, GLfloat b, GLfloat c, GLfloat d ) {
    vec4f res;
    res.R = a;
    res.G = b;
    res.B = c;
    res.A = d;
    return res;
}

static const GLint _N = 6;
static vec3f *_points = NULL;
static vec4f *_colors = NULL;
static bool _initialized = false;

void xyz_initialize()
{
    if (_initialized) return;
    _initialized = true;
    
    puts(__PRETTY_FUNCTION__);
	
    //Vertex vertices[_N] = {
    //    /* X */
    //    { .point={-1, 0, 0}, .color={1, 0, 0} },
    //    { .point={1, 0, 0}, .color={1, 0, 0} },
    //    /* Y */
    //    { .point={0, -1, 0}, .color={0, 1, 0} },
    //    { .point={0, 1, 0}, .color={0, 1, 0} },
    //    /* Z */
    //    { .point={0, 0, -1}, .color={0, 0, 1} },
    //    { .point={0, 0, 1}, .color={0, 0, 1} },
    //};
    
    //printf("%ld\n", sizeof(vertices) / sizeof(GLfloat));
    
    _points = calloc( _N, sizeof(vec3f) );
	_points[0] = vec3fMake( -1, 0, 0 );
	_points[1] = vec3fMake( 1, 0, 0 );
	_points[2] = vec3fMake( 0, -1, 0 );
	_points[3] = vec3fMake( 0, 1, 0 );
	_points[4] = vec3fMake( 0, 0, -1 );
	_points[5] = vec3fMake( 0, 0, 1 );
    
    _colors = calloc( _N, sizeof(vec4f) );
	_colors[0] = vec4fMake( 1, 0, 0, 1 );
	_colors[1] = vec4fMake( 1, 0, 0, 1 );
	_colors[2] = vec4fMake( 0, 1, 0, 1 );
	_colors[3] = vec4fMake( 0, 1, 0, 1 );
	_colors[4] = vec4fMake( 0, 0, 1, 1 );
	_colors[5] = vec4fMake( 0, 0, 1, 1 );
	
    //    Point points[_N_xyz] = {
    //        { -1, 0, 0 },
    //        { 1, 0, 0 },
    //        { 0, -1, 0 },
    //        { 0, 1, 0 },
    //        { 0, 0, -1 },
    //        { 0, 0, 1 }
    //    };
    //    _points_xyz = calloc( _N_xyz, sizeof(Point) );
    //    memcpy(_points_xyz, points, sizeof(points));
}

void xyz_dealloc()
{
    if (!_initialized) return;
    _initialized = false;
    
    printf("%s\n", __PRETTY_FUNCTION__);
    
    free(_points);
    free(_colors);
}

void xyz()
{
    puts(__PRETTY_FUNCTION__);
    xyz_initialize();
    
    glPushMatrix();
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    
    _points = calloc( _N, sizeof(vec3f) );
    _colors = calloc( _N, sizeof(vec4f) );
    
    glVertexPointer( 3, GL_FLOAT, sizeof(vec3f), _points );
    glColorPointer( 4, GL_FLOAT, sizeof(vec4f), _colors );
    //    printf("%s %d\n", __PRETTY_FUNCTION__, _N_xyz);
    glDrawArrays( GL_TRIANGLE_STRIP, 0, _N );
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    glPopMatrix();
}

@interface SphereViewController ()

@end

void printMatrixf (GLfloat *m) {
    printf("---------------------------\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n", m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
}

@implementation SphereViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.

    GLKView *view = (GLKView *)self.view;
    CGRect bounds = view.bounds;
    GLfloat width = bounds.size.width;
    GLfloat height = bounds.size.height;
    
    EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    [EAGLContext setCurrentContext: context];
    view.context = context;
//    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, width, height);
    
//    glEnable(GL_POINT_SMOOTH);
//    glEnable(GL_LINE_SMOOTH);
//    glEnable(GL_DEPTH_TEST);
    
    GLfloat ratio = width/height;
	GLfloat projection[16];
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustumf(-ratio, ratio, -1.0f, 1.0f, 1, 100);
    
//    GLfloat m[16] = {
//        1,0,0,0,
//        0,1,0,0,
//        0,0,1,0,
//        0,0,0,1
//    };
//    glLoadMatrixf(m);
    
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
//    printf("---------------------------\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n", m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
    printMatrixf(projection);
    
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glTranslatef(0, 0, -3);
    
    glFlush();
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - GLKViewDelegate

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClearColor( 1, 1, 1, 1 );
    glClear(GL_COLOR_BUFFER_BIT);
    
    GLfloat projection[16], modelview[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    printMatrixf(projection);
    printMatrixf(modelview);
    
    glMatrixMode(GL_MODELVIEW);
    
    xyz();
    glFlush();
}

- (void)update {
}

@end

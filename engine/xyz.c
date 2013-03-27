/* xyz.c */

#include "xyz.h"

//typedef struct {
//    GLfloat x, y, z;
//} Point;
//
//typedef struct {
//    GLfloat R, G, B;
//} Color;

//typedef union {
//    struct {
//        Point point;
//        Color color;
//    };
//    struct {
//        GLfloat x, y, z;
//        GLfloat R, G, B;
//    };
//} Vertex;

//Point PointMake( GLfloat x, GLfloat y, GLfloat z ) {
//	Point point;
//	point.x = x;
//	point.y = y;
//	point.z = z;
//	return point;
//}
//
//Color ColorMake( GLfloat R, GLfloat G, GLfloat B ) {
//	Color color;
//	color.R = R;
//	color.G = G;
//	color.B = B;
//	return color;
//}

//Vertex VertexMake( Point p, Color c ) {
//	Vertex vertex;
//	vertex.point = p;
//	vertex.color = c;
//	return vertex;
//}

static const GLint _N = 6;
static vec3f *_points = NULL;
static vec3f *_colors = NULL;
static bool _didInitialize = false;

void xyz_initialize()
{
    if (_didInitialize) return;
    _didInitialize = true;
	
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
    
    _colors = calloc( _N, sizeof(vec3f) );
	_colors[0] = vec3fMake( 1, 0, 0 );
	_colors[1] = vec3fMake( 1, 0, 0 );
	_colors[2] = vec3fMake( 0, 1, 0 );
	_colors[3] = vec3fMake( 0, 1, 0 );
	_colors[4] = vec3fMake( 0, 0, 1 );
	_colors[5] = vec3fMake( 0, 0, 1 );
	
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
    if (!_didInitialize) return;
    _didInitialize = false;
    
    printf("%s\n", __PRETTY_FUNCTION__);
    
    free(_points);
    free(_colors);
}

void xyz()
{
    xyz_initialize();
    
    glPushMatrix();
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    
    glVertexPointer( 3, GL_FLOAT, sizeof(vec3f), _points );
    glColorPointer( 3, GL_FLOAT, sizeof(vec3f), _colors );
//    printf("%s %d\n", __PRETTY_FUNCTION__, _N_xyz);
    glDrawArrays( GL_LINES, 0, _N );
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    glPopMatrix();
}


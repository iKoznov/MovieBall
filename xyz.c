/* xyz.c */

#include "xyz.h"

typedef struct {
    GLfloat x, y, z;
} Point;

typedef struct {
    GLfloat R, G, B;
} Color;

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

Point PointMake( GLfloat x, GLfloat y, GLfloat z ) {
	Point point;
	point.x = x;
	point.y = y;
	point.z = z;
	return point;
}

Color ColorMake( GLfloat R, GLfloat G, GLfloat B ) {
	Color color;
	color.R = R;
	color.G = G;
	color.B = B;
	return color;
}

//Vertex VertexMake( Point p, Color c ) {
//	Vertex vertex;
//	vertex.point = p;
//	vertex.color = c;
//	return vertex;
//}

static const GLint _N_xyz = 6;
static Point *_points_xyz = NULL;
static Color *_colors_xyz = NULL;
static bool _didInitialize_xyz = false;

void xyz_initialize()
{
    if (_didInitialize_xyz) return;
    _didInitialize_xyz = true;
	
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
    
    _points_xyz = calloc( _N_xyz, sizeof(Point) );
	_points_xyz[0] = PointMake( -1, 0, 0 );
	_points_xyz[1] = PointMake( 1, 0, 0 );
	_points_xyz[2] = PointMake( 0, -1, 0 );
	_points_xyz[3] = PointMake( 0, 1, 0 );
	_points_xyz[4] = PointMake( 0, 0, -1 );
	_points_xyz[5] = PointMake( 0, 0, 1 );
    
    _colors_xyz = calloc( _N_xyz, sizeof(Color) );
	_colors_xyz[0] = ColorMake( 1, 0, 0 );
	_colors_xyz[1] = ColorMake( 1, 0, 0 );
	_colors_xyz[2] = ColorMake( 0, 1, 0 );
	_colors_xyz[3] = ColorMake( 0, 1, 0 );
	_colors_xyz[4] = ColorMake( 0, 0, 1 );
	_colors_xyz[5] = ColorMake( 0, 0, 1 );
	
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
    if (!_didInitialize_xyz) return;
    _didInitialize_xyz = false;
    
    printf("%s\n", __PRETTY_FUNCTION__);
    
    free(_points_xyz);
    free(_colors_xyz);
}

void xyz()
{
    xyz_initialize();
    
    glPushMatrix();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer( sizeof(Point)/sizeof(GLfloat), GL_FLOAT, sizeof(Point), _points_xyz );
    glColorPointer( sizeof(Color)/sizeof(GLfloat), GL_FLOAT, sizeof(Color), _colors_xyz );
//    printf("%s %d\n", __PRETTY_FUNCTION__, _N_xyz);
    glDrawArrays(GL_LINES, 0, _N_xyz);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    glPopMatrix();
}


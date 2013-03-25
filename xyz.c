/* xyz.c */

#include "xyz.h"

typedef struct {
    GLfloat x, y, z;
} Point;

typedef struct {
    GLfloat R, G, B;
} Color;

typedef union {
    struct {
        Point point;
        Color color;
    };
    struct {
        GLfloat x, y, z;
        GLfloat R, G, B;
    } Vertex;
} Vertex;

static const GLint _N = 6;
static Vertex *_vertices = NULL;
static bool _didInitialize = false;

void xyz_initialize()
{
    if (_didInitialize) return;
    _didInitialize = true;
    
    Vertex vertices[_N] = {
        /* X */
        { .point={ -1, 0, 0 }, .color={ 1, 0, 0 } },
        { .point={1, 0, 0}, .color={1, 0, 0} },
        /* Y */
        { .point={0, -1, 0}, .color={0, 1, 0} },
        { .point={0, 1, 0}, .color={0, 1, 0} },
        /* Z */
        { .point={0, 0, -1}, .color={0, 0, 1} },
        { .point={0, 0, 1}, .color={0, 0, 1} },
    };
    
    printf("%ld\n", sizeof(vertices) / sizeof(GLfloat));
    
    _vertices = malloc(sizeof(vertices));
    memcpy(_vertices, vertices, sizeof(vertices));
}

void xyz_dealloc()
{
    if (!_didInitialize) return;
    _didInitialize = false;
    
    printf("%s\n", __PRETTY_FUNCTION__);
    
    free(_vertices);
}

void xyz()
{
    xyz_initialize();
    
    glPushMatrix();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer( sizeof(Point)/sizeof(GLfloat), GL_FLOAT, sizeof(Vertex), ((GLbyte *)_vertices) + offsetof(Vertex, point) );
    glColorPointer( sizeof(Color)/sizeof(GLfloat), GL_FLOAT, sizeof(Vertex), ((GLbyte *)_vertices) + offsetof(Vertex, color) );
    glDrawArrays(GL_LINES, 0, _N);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    glPopMatrix();
}


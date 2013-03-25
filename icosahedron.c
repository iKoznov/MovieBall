/* icosahedron.c */

#include "icosahedron.h"

typedef struct {
    GLfloat x, y, z;
} Vertex;

static GLint _N = 10;
static Vertex *_vertices = NULL;
static bool _didInitialize = false;

void icosahedron_initialize()
{
    if (_didInitialize) return;
    _didInitialize = true;
    
    GLfloat R = 1.0f;
    GLfloat a = 4.0f / sqrtf(10.0f + 2*sqrtf(5.0f));
    GLfloat h = a * sqrtf( (5 - sqrtf(5.0f)) / 10.0f );
    
    printf("%s\n", __PRETTY_FUNCTION__);
    printf("\tR = %f\n", R);
    printf("\ta = %f\n", a);
    printf("\th = %f\n", h);
    
    _vertices = malloc(_N*sizeof(Vertex));
    
    for (GLint i = 0; i < 5; i++) {
        GLfloat b = i*2.0f*M_PI/5;
        _vertices[i].x = cosf(b);
        _vertices[i].y = h;
        _vertices[i].z = sinf(b);
        printf("%f[%f; %f; %f]\n", b, _vertices[i].x, _vertices[i].y, _vertices[i].z);
    }
    
    for (GLint i = 0; i < 5; i++) {
        GLfloat b = M_PI + i*2.0f*M_PI/5;
        _vertices[i+5].x = cosf(b);
        _vertices[i+5].y = -h;
        _vertices[i+5].z = sinf(b);
        printf("%f[%f; %f; %f]\n", b, _vertices[i].x, _vertices[i].y, _vertices[i].z);
    }
}

void icosahedron_dealloc()
{
    if (!_didInitialize) return;
    _didInitialize = false;
    
    printf("%s\n", __PRETTY_FUNCTION__);
    
    free(_vertices);
}

void icosahedron(GLfloat R)
{
    icosahedron_initialize();
    
    glPushMatrix();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glPointSize(5);
    glVertexPointer(sizeof(Vertex)/sizeof(GLfloat), GL_FLOAT, sizeof(Vertex), _vertices);
    glDrawArrays(GL_POINTS, 0, _N);
    
//    glVertexPointer(sizeof(Vertex)/sizeof(GLfloat), GL_FLOAT, sizeof(Vertex), _vertices);
//    glDrawArrays(GL_LINE_LOOP, 5, 5);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();
}


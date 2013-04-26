/* icosahedron.c */

#include "icosahedron.h"

//typedef struct {
//    GLfloat x, y, z;
//} Point;

static GLint _n;
static GLint _N;
static vec3f *_vertices = NULL;
static bool _didInitialize = false;

void icosahedron_initialize()
{
	GLfloat R, a, h, R5;
	GLint i, j, k;
    vec3f *p;

    if (_didInitialize) return;
    _didInitialize = true;
    
    R = 1.2f;
    _n = 10;
    _N = 2 + 10 * _n*_n;
    a = 4.0f * R / sqrtf(10.0f + 2.0f*sqrtf(5.0f));
    h = a * sqrtf( (5.0f - sqrtf(5.0f)) / 10.0f );
    R5 = a * sqrtf( (5.0f + sqrtf(5.0f)) / 10.0f );
    
    printf("%s\n", __PRETTY_FUNCTION__);
    printf("\tR = %f\n", R);
    printf("\ta = %f\n", a);
    printf("\th = %f\n", h);
    printf("\tR5 = %f\n", R5);
    
    _vertices = p = calloc( _N, sizeof(vec3f) );
    
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

void icosahedron_dealloc()
{
    if (!_didInitialize) return;
    _didInitialize = false;
    
    printf("%s\n", __PRETTY_FUNCTION__);
    
    free(_vertices);
}

void icosahedron()
{
    icosahedron_initialize();
    
    glPushMatrix();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glPointSize(5);
    glVertexPointer( 3, GL_FLOAT, sizeof(vec3f), _vertices );
//    printf("%s %d\n", __PRETTY_FUNCTION__, _N);
    glDrawArrays(GL_POINTS, 0, _N);
    
//    glVertexPointer(sizeof(Vertex)/sizeof(GLfloat), GL_FLOAT, sizeof(Vertex), _vertices);
//    glDrawArrays(GL_LINE_LOOP, 5, 5);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();
}


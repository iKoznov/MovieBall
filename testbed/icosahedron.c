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
    GLfloat hLat, sLon, lat, lon;
	GLint i, j;
    vec3f *p;

    if (_didInitialize) return;
    _didInitialize = true;
    
    R = 1.8f;
    _n = 2;
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
    
    _vertices = p = calloc( _N, sizeof(vec3f) );
    
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
    
//    for (i = 0; i < 5*_n; p++, i++) {
//        GLfloat b = M_PI + i*2.0f*M_PI/(5*_n);
//        (*p).x = R5 * cosf(b);
//        (*p).y = h - R;
//        (*p).z = R5 * sinf(b);
//    }
    
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


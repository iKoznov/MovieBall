/* engine.c */

#include "engine.h"


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


void _printMatrixf (GLfloat *m) {
    printf("---------------------------\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n", m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
}

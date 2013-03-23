#include <math.h>
#include <stdio.h>
#include <locale.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

//#include <GL/glew.h>
//#include <OpenGL/glext.h>
//#include <OpenGL/glu.h>

void reshape(int width, int height);
void display(void);
void timer(int value);

#define _msecs 40
GLfloat _a = 0;
GLfloat _va = 180/M_PI;

static inline void _printMatrixf (GLfloat *m) {
    printf("---------------------------\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n[ %.2f %.2f %.2f %.2f ]\n", m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
}

int main(int argc, char **argv)
{
	puts("<<< SPHERE PROJECT >>>");
    
    char *locale = setlocale(LC_ALL, "Russian");
    printf("LOCALE : %s\n", locale);
    
    glutInit(&argc, argv);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    
    glutCreateWindow("SPHERE");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(_msecs, timer, 0);
    glClearColor(0, 0, 0, 0);
    
    glutMainLoop();
	
	return 0;
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
    GLfloat ratio = (GLfloat)width/(GLfloat)height;
    glFrustum(-ratio, ratio, -1.0f, 1.0f, 1, 100);
    
//    gluOrtho2D(-1, 1, -1, 1);
    
//    GLfloat s[16] = {
//        0.5,0,0,0,
//        0,0.5,0,0,
//        0,0,0.5,0,
//        0,0,0,1
//    };
//    glMultMatrixf(s);
    
    GLfloat projection[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    _printMatrixf(projection);
    
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glTranslatef(0, 0, -3);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    glColor3f(0.0f, 0.0f, 1.0f);
    glLineWidth(1);
    
    glBegin(GL_LINE_LOOP);
    glVertex2f(0, 1.0f);
    glVertex2f(0, -1.0f);
    glVertex2f(2, 0);
    glEnd();
    
    glPushMatrix();
    printf("a = %f\n", _a);
    glRotatef(_a, 0, 1, 0);
    glutWireCube(2);
    glPopMatrix();
    
    glFlush();
    glutSwapBuffers();
}

void timer(int value)
{
    puts("timer");
    _a += _va * _msecs / 1000.0f;
    
    display();
    glutTimerFunc(_msecs, timer, 0);
}

#include <stdio.h>
#include <locale.h>
#include "engine.h"
#include <stdlib.h>
#include <GLUT/glut.h>
#include "icosahedron.h"
#include "xyz.h"
#include "model.h"

//#include <GL/glew.h>
//#include <OpenGL/glext.h>
//#include <OpenGL/glu.h>

void reshape(int width, int height);
void display(void);
void timer(int value);

#define _msecs 10
static GLfloat _a = 0;
static const GLfloat _va = 180.0f/M_PI;
static GLfloat _b = 0;

int main(int argc, char **argv)
{
	puts("<<< SPHERE PROJECT >>>");
//    model();
    
    //char *locale = setlocale(LC_ALL, "Russian");
    //printf("LOCALE : %s\n", locale);
	
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    
    glutCreateWindow("SPHERE");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(_msecs, timer, 0);
    glClearColor(1, 1, 1, 0);
    
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    
    glutMainLoop();
	
	return 0;
}

void reshape(int width, int height)
{
//    puts(__PRETTY_FUNCTION__);
    
    GLfloat ratio = (GLfloat)width/(GLfloat)height;
	GLfloat projection[16];
	
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
    glFrustum(-ratio, ratio, -1.0f, 1.0f, 1, 100);
    
//    gluOrtho2D(-1, 1, -1, 1);
    
//    GLfloat s[16] = {
//        0.5,0,0,0,
//        0,0.5,0,0,
//        0,0,0.5,0,
//        0,0,0,1
//    };
//    glMultMatrixf(s);
    
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    _printMatrixf(projection);
    
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glTranslatef(0, 0, -3);
}

void display()
{
//    puts(__PRETTY_FUNCTION__);
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    glColor3f(1.0f, 0.0f, 1.0f);
    glLineWidth(1);
    
//    glBegin(GL_LINE_LOOP);
//    glVertex2f(0, 1.0f);
//    glVertex2f(0, -1.0f);
//    glVertex2f(2, 0);
//    glEnd();
    
    glPushMatrix();
//    printf("a = %f; b = %f\n", _a, _b);
    glRotatef(_b*180.0f/M_PI, 1, 0, 0);
    glRotatef(_a, 0, 1, 0);
//    xyz();
    icosahedron();
    glutWireCube(2);
    glPopMatrix();
    
    glFlush();
    glutSwapBuffers();
}

void timer(int value)
{
	int time = glutGet(GLUT_ELAPSED_TIME);
//    puts("timer");
    _a += _va * _msecs / 1000.0f;
    _b =  sinf((GLfloat)time/1000.0f) / 2.0f;
    
    display();
    glutTimerFunc(_msecs, timer, 0);
}

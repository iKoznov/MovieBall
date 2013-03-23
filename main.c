#include <stdio.h>
#include <locale.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

//#include <GL/glew.h>
//#include <OpenGL/glext.h>
//#include <OpenGL/glu.h>

void display(void);
void reshape(int width, int height);

int main(int argc, char **argv)
{
	puts("<<< SPHERE PROJECT >>>");
    
    char *locale = setlocale(LC_ALL, "Russian");
    printf("LOCALE : %s\n", locale);
    
    glutInit(&argc, argv);
    glutInitWindowSize(300, 200);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    
    glutCreateWindow("SPHERE");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
	
	return 0;
}

void display()
{
    
}

void reshape(int width, int height)
{
    
}
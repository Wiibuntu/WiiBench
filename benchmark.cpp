#include "glut.h"
#include <math.h>

float angle = 0.0f;
float cubePosX = 0.0f, cubePosY = 0.0f;
float cubeSpeedX = 0.03f, cubeSpeedY = 0.02f;

void initLighting() {
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

void drawCube() {
    glutSolidCube(1.0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glTranslatef(cubePosX, cubePosY, -5.0f);
    glRotatef(angle, 1.0f, 1.0f, 1.0f);
    
    drawCube();

    glutSwapBuffers();
}

void update(int value) {
    angle += 2.0f;
    if (angle > 360) {
        angle -= 360;
    }
    
    cubePosX += cubeSpeedX;
    cubePosY += cubeSpeedY;
    
    if (cubePosX > 2.0f || cubePosX < -2.0f) {
        cubeSpeedX = -cubeSpeedX;
    }
    if (cubePosY > 2.0f || cubePosY < -2.0f) {
        cubeSpeedY = -cubeSpeedY;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cube Benchmark");

    glEnable(GL_DEPTH_TEST);
    initLighting();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}


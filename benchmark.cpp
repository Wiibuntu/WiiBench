#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

float angle = 0.0f;
GLuint texture;

const int treeSegments = 5;
const float trunkHeight = 2.0f;
const float trunkRadius = 0.2f;
const float foliageRadius = 1.0f;
const float foliageHeight = 1.5f;

const int checkImageWidth = 64;
const int checkImageHeight = 64;
GLubyte checkImage[checkImageHeight][checkImageWidth][4];

void makeCheckImage() {
    for (int i = 0; i < checkImageHeight; i++) {
        for (int j = 0; j < checkImageWidth; j++) {
            int c = ((((i & 8) == 0) ^ ((j & 8) == 0))) * 255;
            checkImage[i][j][0] = (GLubyte)c;
            checkImage[i][j][1] = (GLubyte)c;
            checkImage[i][j][2] = (GLubyte)c;
            checkImage[i][j][3] = (GLubyte)255;
        }
    }
}

void initTexture() {
    makeCheckImage();
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
    glEnable(GL_TEXTURE_2D);
}

void initRayTracedLighting() {
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat light_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
}

void drawTree() {
    // Draw trunk
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.0f, -trunkHeight / 2.0f, 0.0f);
    gluCylinder(quad, trunkRadius, trunkRadius, trunkHeight, 30, 30);
    glPopMatrix();

    // Draw foliage (stack of cones)
    for (int i = 0; i < treeSegments; i++) {
        glPushMatrix();
        glTranslatef(0.0f, trunkHeight + i * foliageHeight / 2.5f, 0.0f);
        glutSolidCone(foliageRadius - i * 0.2f, foliageHeight, 30, 30);
        glPopMatrix();
    }

    gluDeleteQuadric(quad);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, -1.0f, -6.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture);
    drawTree();
    
    glutSwapBuffers();
}

void update(int value) {
    angle += 1.0f;
    if (angle > 360) {
        angle -= 360;
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
    glutCreateWindow("Rotating High Poly Tree with Ray Traced Lighting");

    glEnable(GL_DEPTH_TEST);
    initRayTracedLighting();
    initTexture();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}


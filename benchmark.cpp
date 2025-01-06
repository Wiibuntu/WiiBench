#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

float angle = 0.0f;
GLuint texture;
const int numPolygons = 12;  // 1/4 the original number (50/4)

const int checkImageWidth = 64;
const int checkImageHeight = 64;
GLubyte checkImage[checkImageHeight][checkImageWidth][4];

struct Polygon {
    float x, y, z;
    float size;
    float rotationSpeed;
} polygons[numPolygons];

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

void generatePolygons() {
    for (int i = 0; i < numPolygons; i++) {
        polygons[i].x = ((float)rand() / RAND_MAX) * 4.0f - 2.0f;
        polygons[i].y = ((float)rand() / RAND_MAX) * 4.0f - 2.0f;
        polygons[i].z = ((float)rand() / RAND_MAX) * -10.0f - 5.0f;
        polygons[i].size = ((float)rand() / RAND_MAX) * 1.0f + 0.5f;
        polygons[i].rotationSpeed = ((float)rand() / RAND_MAX) * 5.0f;
    }
}

void drawPolygon(float size) {
    glutSolidDodecahedron();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    for (int i = 0; i < numPolygons; i++) {
        glPushMatrix();
        glTranslatef(polygons[i].x, polygons[i].y, polygons[i].z);
        glRotatef(angle * polygons[i].rotationSpeed, 1.0f, 1.0f, 1.0f);
        glScalef(polygons[i].size, polygons[i].size, polygons[i].size);
        drawPolygon(polygons[i].size);
        glPopMatrix();
    }
    
    glutSwapBuffers();
}

void update(int value) {
    angle += 2.0f;
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
    glutCreateWindow("Ray Traced Polygon Scene Benchmark");

    glEnable(GL_DEPTH_TEST);
    initRayTracedLighting();
    initTexture();
    generatePolygons();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}


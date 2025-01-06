#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <vector>

float angle = 0.0f;
float lightAngle = 0.0f;
GLuint texture;
const int worldSize = 50;
float terrain[worldSize][worldSize];

const int checkImageWidth = 64;
const int checkImageHeight = 64;
GLubyte checkImage[checkImageHeight][checkImageWidth][4];

float perlinNoise(int x, int y) {
    float scale = 0.1f;
    return (float)(sin(x * scale) * cos(y * scale) * 10);
}

void generateTerrain() {
    for (int x = 0; x < worldSize; x++) {
        for (int y = 0; y < worldSize; y++) {
            terrain[x][y] = perlinNoise(x, y);
        }
    }
}

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
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
}

void updateLightPosition() {
    GLfloat light_position[] = { 10.0f * sin(lightAngle), 10.0f, 10.0f * cos(lightAngle), 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void drawBlock(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawTerrain() {
    for (int x = 0; x < worldSize; x++) {
        for (int y = 0; y < worldSize; y++) {
            float height = terrain[x][y];
            drawBlock(x - worldSize / 2, height, y - worldSize / 2);
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, -10.0f, -50.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    updateLightPosition();
    
    glBindTexture(GL_TEXTURE_2D, texture);
    drawTerrain();

    glutSwapBuffers();
}

void update(int value) {
    angle += 1.0f;
    if (angle > 360) {
        angle -= 360;
    }

    lightAngle += 0.01f;
    if (lightAngle > 2 * M_PI) {
        lightAngle -= 2 * M_PI;
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
    glutCreateWindow("Minecraft-Style Terrain with Rotating Light and Perlin Noise");

    glEnable(GL_DEPTH_TEST);
    initRayTracedLighting();
    initTexture();
    generateTerrain();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}


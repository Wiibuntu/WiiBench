#include <GL/glut.h>
#include <math.h>

float angle = 0.0f;
float spherePosX = 0.0f, spherePosY = 0.0f;
float sphereSpeedX = 0.03f, sphereSpeedY = 0.02f;
GLuint texture;

const int checkImageWidth = 64;
const int checkImageHeight = 64;
GLubyte checkImage[checkImageHeight][checkImageWidth][4];

void makeCheckImage(void) {
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

void drawSphere() {
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, 1.0, 64, 64);
    gluDeleteQuadric(quadric);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glTranslatef(spherePosX, spherePosY, -5.0f);
    glRotatef(angle, 1.0f, 1.0f, 1.0f);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    drawSphere();

    glutSwapBuffers();
}

void update(int value) {
    angle += 2.0f;
    if (angle > 360) {
        angle -= 360;
    }
    
    spherePosX += sphereSpeedX;
    spherePosY += sphereSpeedY;
    
    if (spherePosX > 2.0f || spherePosX < -2.0f) {
        sphereSpeedX = -sphereSpeedX;
    }
    if (spherePosY > 2.0f || spherePosY < -2.0f) {
        sphereSpeedY = -sphereSpeedY;
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
    glutCreateWindow("Sphere Benchmark with Checkerboard Texture");

    glEnable(GL_DEPTH_TEST);
    initLighting();
    initTexture();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}


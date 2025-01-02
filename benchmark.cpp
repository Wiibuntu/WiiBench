#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

// Vertex Shader Source
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

// Fragment Shader Source
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

struct Shape {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

Shape createCube() {
    return {
        { // Vertices
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
        },
        { // Indices
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            0, 1, 5, 5, 4, 0,
            2, 3, 7, 7, 6, 2,
            0, 3, 7, 7, 4, 0,
            1, 2, 6, 6, 5, 1
        }
    };
}

Shape createPyramid() {
    return {
        { // Vertices
            0.0f,  0.5f,  0.0f,
           -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f
        },
        { // Indices
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 1,
            1, 2, 3,
            3, 4, 1
        }
    };
}

Shape createSphere() {
    const int stacks = 20;
    const int slices = 20;
    const float radius = 0.5f;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= stacks; ++i) {
        float phi = i * M_PI / stacks;
        for (int j = 0; j <= slices; ++j) {
            float theta = j * 2 * M_PI / slices;
            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = (i * (slices + 1)) + j;
            int second = first + slices + 1;
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    return {vertices, indices};
}

Shape createCylinder() {
    const int segments = 20;
    const float radius = 0.5f;
    const float height = 1.0f;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= segments; ++i) {
        float theta = i * 2 * M_PI / segments;
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        vertices.push_back(x);
        vertices.push_back(-height / 2);
        vertices.push_back(z);
        vertices.push_back(x);
        vertices.push_back(height / 2);
        vertices.push_back(z);
    }

    for (int i = 0; i < segments; ++i) {
        int base = i * 2;
        indices.push_back(base);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 1);
        indices.push_back(base + 3);
        indices.push_back(base + 2);
    }

    return {vertices, indices};
}

Shape createCone() {
    const int segments = 20;
    const float radius = 0.5f;
    const float height = 1.0f;
    std::vector<float> vertices = {0.0f, height / 2, 0.0f};
    std::vector<unsigned int> indices;

    for (int i = 0; i <= segments; ++i) {
        float theta = i * 2 * M_PI / segments;
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        vertices.push_back(x);
        vertices.push_back(-height / 2);
        vertices.push_back(z);
    }

    for (int i = 1; i <= segments; ++i) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    return {vertices, indices};
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    // Implement camera rotation logic
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Shape Selector", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouseCallback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::vector<Shape> shapes = {createCube(), createPyramid(), createSphere(), createCylinder(), createCone()};
    int currentShapeIndex = 0;

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

     while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        Shape currentShape = shapes[currentShapeIndex];

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, currentShape.vertices.size() * sizeof(float), currentShape.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentShape.indices.size() * sizeof(unsigned int), currentShape.indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    return 0; }


#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

void glFramebufferSizeCallback(GLFWwindow* window, const int width, const int height) {
    glViewport(0,0,width ,height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_F)) {
       glClearColor(0.0f,0.0f,0.0f,1.0f);
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
       glClear(GL_COLOR_BUFFER_BIT);
    } else {
        glClearColor(0.2f,0.3f,0.3f,1.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

float VERTICESTRIG[] = {
    //triangle #1
    -1.0f, -1.0f,  0.0f,
     1.0f,  0.0f,  0.0f,

     1.0f, -1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,

     0.0f,  1.0f,  0.0f,
     0.0f,  0.0f,  1.0f,
};

int INDECIES[] = {
    0,1,2
};


int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }

    const int width = 800;
    const int height = 600;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Arch OpenGL", nullptr, nullptr);

    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD init failed\n";
        return -1;
    }

    glViewport( 0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, glFramebufferSizeCallback);

    unsigned int VBOs[2], VAOs[2], EBOs[2];
    glGenBuffers(2, VBOs);
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, EBOs);


    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICESTRIG), VERTICESTRIG, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDECIES), INDECIES, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3, GL_FLOAT, GL_TRUE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader = Shader("/home/crystal/CLionProjects/OpenGLFun/src/shaders/vertex.glsl", "/home/crystal/CLionProjects/OpenGLFun/src/shaders/fragment.glsl");

    shader.setFloat("OFFSET", 0.0);

    float y = 0;

    while (!glfwWindowShouldClose(window)) {
        y = std::fmod(glfwGetTime()*0.5*std::numbers::pi, 4.0)-2.0f;
        shader.setFloat("OFFSET", y);
        processInput(window);
        shader.use();

        //std::cout << y << std::endl;

        glBindVertexArray(VAOs[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
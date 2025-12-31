#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


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
    -0.5f, -0.5f,  0.0f,
     1.0f,  0.0f,  0.0f,
     1.0f, 1.0f,

     0.5f, -0.5f,  0.0f,
     0.0f,  1.0f,  0.0f,
     1.0f,  0.0f,

    -0.5f,  0.5f,  0.0f,
     0.0f,  0.0f,  1.0f,
     0.0f,  1.0f,

     0.5f, 0.5f, 0.0f,
     0.0f, 0.0f, 0.0f,
     0.0f, 0.0f

};

int INDECIES[] = {
    0,1,2, 2,3,1
};


int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }

    const int WINDOWWIDTH= 800;
    const int WINDOWHEIGHT = 600;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "Arch OpenGL", nullptr, nullptr);

    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD init failed\n";
        return -1;
    }

    glViewport( 0, 0, WINDOWWIDTH, WINDOWHEIGHT);

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

    const unsigned int Stride = 8*sizeof(float);

    glVertexAttribPointer(0,3, GL_FLOAT, GL_TRUE, Stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  Stride, (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Stride, (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    Shader shader = Shader("/home/crystal/CLionProjects/OpenGLFun/src/shaders/vertex.glsl", "/home/crystal/CLionProjects/OpenGLFun/src/shaders/fragment.glsl");

    shader.setFloat("OFFSET", 0.0);

    float y = 0;



    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int texWidth, texHeight, nrChannels;
    unsigned char* data = stbi_load("../textures/wall.jpg", &texWidth, &texHeight, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "texture loaded successfully" << std::endl;
    } else {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);

    while (!glfwWindowShouldClose(window)) {
        y = std::fmod(glfwGetTime()*0.5*std::numbers::pi, 4.0)-2.0f;
        shader.setFloat("OFFSET", y);
        processInput(window);


        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        shader.use();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
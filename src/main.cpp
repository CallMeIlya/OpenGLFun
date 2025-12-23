#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

     0.5f, -0.5f,  0.0f,
     0.0f,  1.0f,  0.0f,

     0.0f,  0.5f,  0.0f,
     0.0f,  0.0f,  1.0f,

     0.5f,  0.5f,  0.0f,
     0.5f,  0.5f,  0.5f
};

int INDECIES[] = {
    0,1,2
};

const char *VertexshaderSource =
    "#version 460 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec3 aCol;\n"
    "out vec4 vertexColor;\n"
    "void main() {\n"
    "gl_Position = vec4(aPos, 1.0);\n"
    "vertexColor = vec4(aCol, 1.0);\n"
    "}\0";
const char *FragmentshaderSource =
    "#version 460 core\n"
    "in vec4 vertexColor;\n"
    "uniform vec4 timeColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "FragColor = vertexColor;\n"
    "}\0";

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

    unsigned int vertexshader = glCreateShader(GL_VERTEX_SHADER); //checks if shader is created correctly.
    if (vertexshader == 0) { //0 indicates an error creating the shader.
        return -2;
    }
    glShaderSource(vertexshader, 1, &VertexshaderSource, NULL);
    glCompileShader(vertexshader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
    if (!success) { //checks if compilation went well
        glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
        std::cout << "vertexshader failed compilation\n";
        for (char i : infoLog) {
            std::cout << i;
        }
    }

    unsigned int fragmentshader = glCreateShader(GL_FRAGMENT_SHADER); //checks if shader is created correctly
    if (fragmentshader == 0) {
        return -3;
    }
    glShaderSource(fragmentshader, 1, &FragmentshaderSource, NULL);
    glCompileShader(fragmentshader);

    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentshader, 512, NULL, infoLog);
        std::cout << "fragmentshader failed compilation\n";
        for (char i : infoLog) {
            std::cout << infoLog[i];
        }
    }

    unsigned int shaderPrograms[2] = {glCreateProgram(), glCreateProgram()};
    if (shaderPrograms[0]== 0) {
        return -4;
    }

    glAttachShader(shaderPrograms[0], vertexshader);
    glAttachShader(shaderPrograms[0], fragmentshader);
    glLinkProgram(shaderPrograms[0]);

    glAttachShader(shaderPrograms[1], vertexshader);
    glAttachShader(shaderPrograms[1], fragmentshader);
    glLinkProgram(shaderPrograms[1]);

    glGetProgramiv(shaderPrograms[0], GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderPrograms[0], 512, NULL, infoLog);
        for (char i : infoLog) {
            std::cout << i;
        }
    }

    glDeleteShader(fragmentshader); glDeleteShader(vertexshader); //deletes shaders since once compiled we don't need them anymore

    glUseProgram(shaderPrograms[0]);

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

    /*
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES2), VERTICES2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    */

    while (!glfwWindowShouldClose(window)) {
        float RedColorVal = std::sin(std::numbers::pi*glfwGetTime())+0.5;
        float GreenColorVal = std::sin(std::numbers::pi*(0.25+glfwGetTime()))+0.5;
        float BlueColorVal = std::cos(std::numbers::pi*(0.5+glfwGetTime()))+0.5;

        processInput(window);

        int vertexColorLocation = glGetUniformLocation(shaderPrograms[0], "timeColor");
        glUniform4f(vertexColorLocation, RedColorVal, GreenColorVal, BlueColorVal, 0);

        glUseProgram(shaderPrograms[0]);
        glBindVertexArray(VAOs[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
        glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
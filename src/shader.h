//
// Created by crystal on 24-12-2025.
//

#ifndef OPENGL_SHADERD_H
#define OPENGL_SHADERD_H

#include<glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        //causes the vShaderFile and fShaderFile streams to actually throw errors.
        //If the badbit or failbits are true, then this will throw an error.

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << e.what() << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        //vertexshader innitialization.
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        int success;
        char infoLog[512];
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

        if (!success) {
            glad_glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            std::cout << "ERROR::VERTEX_SHADER::COMPILATION_FAILED" << std::endl;
        }

        //fragment shader innitialization.
        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

        if (!success) {
            glad_glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            std::cout << "ERROR::FRAGMENT_SHADER::COMPILATION_FAILED" << std::endl;
        }
        this -> ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            std::cout << "ERROR::PROGRAM::LINKING_FAILED" << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };

    void use() {
        glUseProgram(this -> ID);
    };
    void setBool(const std::string& name, bool value) {
        glUniform1i(glGetUniformLocation(this -> ID, name.c_str()), (int)value);
    };
    void setInt(const std::string& name, int value) {
        glUniform1i(glGetUniformLocation(this -> ID, name.c_str()), (int)value);
    }
    void setFloat(const std::string& name, float value) {
        glUniform1f(glGetUniformLocation(this -> ID, name.c_str()), value);
    };
};


#endif //OPENGL_SHADERD_H
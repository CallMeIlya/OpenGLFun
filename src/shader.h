//
// Created by crystal on 24-12-2025.
//

#ifndef OPENGL_SHADERD_H
#define OPENGL_SHADERD_H
#pragma once

#include<glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();

    void setBool(const std::string& name, bool value);

    void setInt(const std::string& name, int value);

    void setFloat(const std::string& name, float value);
};


#endif //OPENGL_SHADERD_H
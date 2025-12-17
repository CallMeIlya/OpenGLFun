cmake_minimum_required(VERSION 4.1)
project(OpenGL)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenGL
        src/main.cpp
        src/glad.c
)

target_include_directories(OpenGL PRIVATE include)

target_link_libraries(OpenGL
    OpenGL::GL
    glfw
)

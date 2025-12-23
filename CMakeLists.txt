cmake_minimum_required(VERSION 3.31.6)
project(OpenGL LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenGL
        src/glad.c
        src/main.cpp
)

target_include_directories(OpenGL PRIVATE include)

target_link_libraries(OpenGL
    OpenGL::GL
    glfw
)

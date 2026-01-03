#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aTex;

uniform float OFFSET;

out vec4 vertexColor;
out vec2 texPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexColor = vec4(aPos, 1.0);
    texPos = aTex;
}

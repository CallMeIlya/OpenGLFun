#version 460 core

in vec4 vertexColor;

out vec4 FragColor;

void main() {
    FragColor = vertexColor; //the reason the bottom left corner values are black is because you can't have negative colors.
    //negative colors get set to 0 or clamped.
}

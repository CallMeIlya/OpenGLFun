#version 460 core

in vec4 vertexColor;
in vec2 texPos;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main() {
    FragColor = texture(ourTexture, texPos);
}

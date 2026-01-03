#version 460 core

in vec4 vertexColor;
in vec2 texPos;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

out vec4 FragColor;

void main() {
    //FragColor = texture(Texture2, texPos);
    vec2 texPosFlip = texPos;
    texPosFlip.x = -texPosFlip.x;

    FragColor = mix(texture(Texture1, 1.0*texPos), texture(Texture2, 1.0*texPos), 0.7);
}

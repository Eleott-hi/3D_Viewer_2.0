#version 410 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main() {
    FragColor = texture(skybox, TexCoords);

    FragColor = vec4(0.48f, 0.58f, 0.07f, 1.0f);
}
#version 410 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D diffuse;

void main() {  //
  FragColor = vec4(vec3(texture(diffuse, TexCoords)), 1);
}
#version 410 core

out vec4 FragColor;

struct Material {
  vec4 color;
};

uniform Material material;

void main() {
  FragColor = vec4(vec3(material.color), 1);
}

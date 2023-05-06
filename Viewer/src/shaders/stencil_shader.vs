#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() {
  gl_Position = proj * view * model * vec4(aPos + aNormal * 0.1, 1.0);
}
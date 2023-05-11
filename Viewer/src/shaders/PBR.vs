#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main() {

  TexCoords = aTexCoords;
  WorldPos = vec3(Model * vec4(aPos, 1.0));
  Normal = mat3(transpose(inverse(Model))) * aNormal;

  gl_Position = Projection * View * vec4(WorldPos, 1.0);
}

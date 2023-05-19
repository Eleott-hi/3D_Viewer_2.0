#version 410 core

in vec3 WorldPos;
in vec3 Normal;
out vec4 FragColor;

struct Material {
  vec4 color;
  float shininess;
};

uniform vec3 viewPos;
uniform Material material;

vec3 CalcLight(vec3 N, vec3 V, vec3 diffuseColor, vec3 specularColor, float shininess);

//! import include/light.glsl

void main() {

  vec3 light = CalcLight(//
  normalize(Normal), //
  normalize(viewPos - WorldPos), //
  material.color.rgb, //
  material.color.rgb, //
  material.shininess//
  );

  FragColor = vec4(light, 1.0f);
}
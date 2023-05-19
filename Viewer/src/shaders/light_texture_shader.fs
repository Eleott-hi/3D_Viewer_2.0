#version 410 core

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;
out vec4 FragColor;

struct Material {
  sampler2D diffuseMap;
  sampler2D specularMap;
  float shininess;
};

uniform vec3 viewPos;
uniform Material material;

vec3 CalcLight(vec3 N, vec3 V, vec3 diffuseColor, vec3 specularColor, float shininess);

//! import include/light.glsl

void main() {

  vec3 light = CalcLight(//
  normalize(Normal), //
  normalize(viewPos - WorldPos),//
  vec3(texture(material.diffuseMap, TexCoords)), //
  vec3(texture(material.specularMap, TexCoords)), //
  material.shininess//
  );

  FragColor = vec4(light, 1.0f);
}

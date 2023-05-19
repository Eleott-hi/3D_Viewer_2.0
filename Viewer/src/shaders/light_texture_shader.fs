#version 410 core

in vec3 FragPos;
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

vec3 CalcLightColor(vec3 norm, vec3 viewDir, vec3 worldPos, vec3 diffuseColor, vec3 specularColor, float shininess);

//! import include/test_include.fs

void main() {
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 diffuse = vec3(texture(material.diffuseMap, TexCoords));
  vec3 specular = vec3(texture(material.specularMap, TexCoords));

  vec3 light = CalcLightColor(norm, viewDir, FragPos, diffuse, specular, material.shininess);

  FragColor = vec4(light, 1.0f);
}

#version 420 core

in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

struct Material {
  float shininess;
};

struct PointLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform vec4 u_Color;
uniform vec3 u_viewPos;
uniform PointLight u_pointLights[100];
uniform int u_pointLightsCount;
uniform Material u_material;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(u_viewPos - FragPos);
  vec3 light;
  for (int i = 0; i < u_pointLightsCount; i++) {
    light += CalcPointLight(u_pointLights[i], norm, FragPos, viewDir);
  }
  FragColor = vec4(light, 1.0f);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = light.position - fragPos;
  vec3 nlightDir = normalize(lightDir);
  vec3 reflectDir = reflect(-nlightDir, normal);

  float diff = max(dot(normal, nlightDir), 0.0);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
  float distance = length(lightDir);
  float attenuation = light.constant + light.linear * distance;

  vec3 ambient = light.ambient * u_Color.xyz;
  vec3 diffuse = light.diffuse * diff * u_Color.xyz;
  vec3 specular = light.specular * spec * u_Color.xyz;

  ambient /= attenuation;
  diffuse /= attenuation;
  specular /= attenuation;
  return (ambient + diffuse + specular);
}

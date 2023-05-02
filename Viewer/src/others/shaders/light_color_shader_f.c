#version 420 core

in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

struct Material {
  float shininess;
};

struct DirLight {
  int light_index;
  int attenuation_index;

  vec3 direction;
};

struct PointLight {
  int light_index;
  int attenuation_index;

  vec3 position;
};

struct SpotLight {
  int light_index;
  int attenuation_index;

  vec3 position;
  vec3 direction;
  float inner_cone;
  float outer_cone;
};

struct Light {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Attenuation {
  float constant;
  float linear;
  float quadratic;
};

uniform vec4 u_Color;
uniform vec3 u_viewPos;

uniform DirLight u_dirLights[20];
uniform PointLight u_pointLights[20];
uniform SpotLight u_spotLights[20];
uniform Light u_lights[60];
uniform Attenuation u_attenuations[60];

uniform int u_dirLightCount;
uniform int u_pointLightCount;
uniform int u_spotLightCount;

uniform Material u_material;

vec3 CalcPointLight(int i, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDirectLight(int i, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(int i, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(u_viewPos - FragPos);
  vec3 light;

  for (int i = 0; i < u_dirLightCount; i++) {
    light += CalcDirectLight(i, norm, FragPos, viewDir);
  }

  for (int i = 0; i < u_pointLightCount; i++) {
    light += CalcPointLight(i, norm, FragPos, viewDir);
  }

  for (int i = 0; i < u_spotLightCount; i++) {
    light += CalcSpotLight(i, norm, FragPos, viewDir);
  }

  FragColor = vec4(light, 1.0f);
}

vec3 CalcSpotLight(int i, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 tmp = u_spotLights[i].position - fragPos;
  vec3 lightDir = normalize(tmp);
  vec3 reflectDir = reflect(-lightDir, normal);

  float diff = max(dot(normal, lightDir), 0.0);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

  float theta = dot(lightDir, normalize(-u_spotLights[i].direction));
  float epsilon = u_spotLights[i].inner_cone - u_spotLights[i].outer_cone;
  float intensity =
      clamp((theta - u_spotLights[i].outer_cone) / epsilon, 0.0, 1.0);

  vec3 ambient = u_lights[u_spotLights[i].light_index].ambient * u_Color.xyz;
  vec3 diffuse =
      u_lights[u_spotLights[i].light_index].diffuse * diff * u_Color.xyz;
  vec3 specular =
      u_lights[u_spotLights[i].light_index].specular * spec * u_Color.xyz;

  ambient *= intensity;
  diffuse *= intensity;
  specular *= intensity;

  if (u_spotLights[i].attenuation_index != -1) {
    float distance = length(tmp);
    float att =
        u_attenuations[u_spotLights[i].attenuation_index].constant +
        u_attenuations[u_spotLights[i].attenuation_index].linear * distance;

    ambient /= att;
    diffuse /= att;
    specular /= att;
  }

  return (ambient + diffuse + specular);
}

vec3 CalcPointLight(int i, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = u_pointLights[i].position - fragPos;
  vec3 nlightDir = normalize(lightDir);
  vec3 reflectDir = reflect(-nlightDir, normal);

  float diff = max(dot(normal, nlightDir), 0.0);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

  Light light = u_lights[u_pointLights[i].light_index];
  vec3 ambient = light.ambient * u_Color.xyz;
  vec3 diffuse = light.diffuse * diff * u_Color.xyz;
  vec3 specular = light.specular * spec * u_Color.xyz;

  if (u_pointLights[i].attenuation_index != -1) {
    float distance = length(lightDir);
    float att =
        u_attenuations[u_pointLights[i].attenuation_index].constant +
        u_attenuations[u_pointLights[i].attenuation_index].linear * distance;

    ambient /= att;
    diffuse /= att;
    specular /= att;
  }

  return (ambient + diffuse + specular);
}

vec3 CalcDirectLight(int i, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(-u_dirLights[i].direction);
  vec3 reflectDir = reflect(-lightDir, normal);

  float diff = max(dot(normal, lightDir), 0.0);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

  vec3 ambient = u_lights[u_dirLights[i].light_index].ambient * u_Color.xyz;
  vec3 diffuse =
      u_lights[u_dirLights[i].light_index].diffuse * diff * u_Color.xyz;
  vec3 specular =
      u_lights[u_dirLights[i].light_index].specular * spec * u_Color.xyz;

  return (ambient + diffuse + specular);
}
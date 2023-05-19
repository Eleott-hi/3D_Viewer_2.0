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

uniform Light lights[60];
uniform DirLight dirLights[20];
uniform SpotLight spotLights[20];
uniform PointLight pointLights[20];
uniform Attenuation attenuations[60];

uniform int dirLightCount;
uniform int pointLightCount;
uniform int spotLightCount;

vec3 CalcSpotLight(
  int i,
  vec3 normal,
  vec3 viewDir,
  vec3 worldPos,
  vec3 diffuseColor,
  vec3 specularColor,
  float shininess
) {
  vec3 tmp = spotLights[i].position - worldPos;
  vec3 lightDir = normalize(tmp);
  vec3 reflectDir = reflect(-lightDir, normal);

  float diff = max(dot(normal, lightDir), 0.0);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

  float theta = dot(lightDir, normalize(-spotLights[i].direction));
  float epsilon = spotLights[i].inner_cone - spotLights[i].outer_cone;
  float intensity = clamp((theta - spotLights[i].outer_cone) / epsilon, 0.0, 1.0);

  vec3 ambient = lights[spotLights[i].light_index].ambient * diffuseColor;
  vec3 diffuse = lights[spotLights[i].light_index].diffuse * diff * diffuseColor;
  vec3 specular = lights[spotLights[i].light_index].specular * spec * specularColor;

  ambient *= intensity;
  diffuse *= intensity;
  specular *= intensity;

  if(spotLights[i].attenuation_index != -1) {
    float distance = length(tmp);
    float att = attenuations[spotLights[i].attenuation_index].constant +
      attenuations[spotLights[i].attenuation_index].linear * distance;

    ambient /= att;
    diffuse /= att;
    specular /= att;
  }

  return (ambient + diffuse + specular);
}

vec3 CalcPointLight(
  int i,
  vec3 normal,
  vec3 viewDir,
  vec3 worldPos,
  vec3 diffuseColor,
  vec3 specularColor,
  float shininess
) {
  vec3 lightDir = pointLights[i].position - worldPos;
  vec3 nlightDir = normalize(lightDir);
  vec3 reflectDir = reflect(-nlightDir, normal);

  float diff = max(dot(normal, nlightDir), 0.0);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

  vec3 ambient = lights[pointLights[i].light_index].ambient * diffuseColor;
  vec3 diffuse = lights[pointLights[i].light_index].diffuse * diff * diffuseColor;
  vec3 specular = lights[pointLights[i].light_index].specular * spec * specularColor;

  if(pointLights[i].attenuation_index != -1) {
    float distance = length(lightDir);

    float att = attenuations[pointLights[i].attenuation_index].constant +
      attenuations[pointLights[i].attenuation_index].linear * distance;

    ambient /= att;
    diffuse /= att;
    specular /= att;
  }

  return ambient + diffuse + specular;
}

vec3 CalcDirectLight(
  int i,
  vec3 normal,
  vec3 viewDir,
  vec3 diffuseColor,
  vec3 specularColor,
  float shininess
) {
  vec3 lightDir = normalize(-dirLights[i].direction);
  vec3 reflectDir = reflect(-lightDir, normal);

  float diff = max(dot(normal, lightDir), 0.0);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

  vec3 ambient = lights[dirLights[i].light_index].ambient * diffuseColor;
  vec3 diffuse = lights[dirLights[i].light_index].diffuse * diff * diffuseColor;
  vec3 specular = lights[dirLights[i].light_index].specular * spec * specularColor;

  return ambient + diffuse + specular;
}

vec3 CalcLightColor(
  vec3 norm,
  vec3 viewDir,
  vec3 worldPos,
  vec3 diffuseColor,
  vec3 specularColor,
  float shininess
) {
  vec3 light;

  for(int i = 0; i < dirLightCount; i++) light += CalcDirectLight(i, norm, viewDir, diffuseColor, specularColor, shininess);
  for(int i = 0; i < pointLightCount; i++) light += CalcPointLight(i, norm, viewDir, worldPos, diffuseColor, specularColor, shininess);
  for(int i = 0; i < spotLightCount; i++) light += CalcSpotLight(i, norm, viewDir, worldPos, diffuseColor, specularColor, shininess);

  return light;
}

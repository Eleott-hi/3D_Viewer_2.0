const float PI = 3.14159265359;

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
uniform int spotLightCount;
uniform int pointLightCount;

// ----------------------------------------------------------------------------

float CalcAttenuation(int index, vec3 L) {

  if(index == -1)
    return 1.0;

  float distance = length(L);
  float att = attenuations[index].constant + attenuations[index].linear * distance;

  return 1.0 / att;
}

// ----------------------------------------------------------------------------

float SpotIntensity(int i, vec3 nL) {
  float theta = dot(nL, normalize(-spotLights[i].direction));
  float epsilon = spotLights[i].inner_cone - spotLights[i].outer_cone;
  float intensity = clamp((theta - spotLights[i].outer_cone) / epsilon, 0.0, 1.0);

  return intensity;
}

// ----------------------------------------------------------------------------

vec3 GetLight(
  int index,
  vec3 nL,
  vec3 N,
  vec3 V,
  vec3 diffuseColor,
  vec3 specularColor,
  float shininess
) {
  vec3 R = reflect(-nL, N);

  float diff = max(dot(N, nL), 0.0);
  float spec = pow(max(dot(V, R), 0.0), shininess);

  vec3 ambient = lights[index].ambient * diffuseColor;
  vec3 diffuse = lights[index].diffuse * diff * diffuseColor;
  vec3 specular = lights[index].specular * spec * specularColor;

  return ambient + diffuse + specular;
}

// ----------------------------------------------------------------------------

vec3 CalcSpotLight(
  int i,
  vec3 N,
  vec3 V,

  vec3 diffuseColor,
  vec3 specularColor,
  float shininess
) {
  int index = spotLights[i].light_index;
  int attIndex = spotLights[i].attenuation_index;
  vec3 L = spotLights[i].position - WorldPos;
  vec3 nL = normalize(L);

  vec3 light = GetLight(index, nL, N, V, diffuseColor, specularColor, shininess);

  float attenuation = CalcAttenuation(attIndex, L);
  float intensity = SpotIntensity(i, nL);

  return light * intensity * attenuation;
}

// ----------------------------------------------------------------------------

vec3 CalcPointLight(
  int i,
  vec3 N,
  vec3 V,

  vec3 diffuseColor,
  vec3 specularColor,
  float shininess
) {
  int index = pointLights[i].light_index;
  int attIndex = pointLights[i].attenuation_index;

  vec3 L = pointLights[i].position - WorldPos;
  vec3 nL = normalize(L);

  vec3 light = GetLight(index, nL, N, V, diffuseColor, specularColor, shininess);
  float attenuation = CalcAttenuation(attIndex, L);

  return light * attenuation;
}

// ----------------------------------------------------------------------------

vec3 CalcDirectLight(
  int i,
  vec3 N,
  vec3 V,
  vec3 diffuseColor,
  vec3 specularColor,
  float shininess
) {
  int index = dirLights[i].light_index;
  vec3 nL = normalize(-dirLights[i].direction);
  vec3 light = GetLight(index, nL, N, V, diffuseColor, specularColor, shininess);

  return light;
}

// ----------------------------------------------------------------------------

vec3 CalcLight(
  vec3 normal,
  vec3 viewDir,

  vec3 diffuseColor,
  vec3 specularColor,
  float shininess
) {
  vec3 light;

  for(int i = 0; i < dirLightCount; i++) light += CalcDirectLight(i, normal, viewDir, diffuseColor, specularColor, shininess);
  for(int i = 0; i < pointLightCount; i++) light += CalcPointLight(i, normal, viewDir, diffuseColor, specularColor, shininess);
  for(int i = 0; i < spotLightCount; i++) light += CalcSpotLight(i, normal, viewDir, diffuseColor, specularColor, shininess);

  return light;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

float DistributionGGX(vec3 N, vec3 H, float roughness) {
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float nom = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return nom / denom;
}

// ----------------------------------------------------------------------------

float GeometrySchlickGGX(float NdotV, float roughness) {
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;

  float nom = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return nom / denom;
}

// ----------------------------------------------------------------------------

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
  float ggx1 = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
}

// ----------------------------------------------------------------------------

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// ----------------------------------------------------------------------------

vec3 MainPatternPBR(vec3 N, vec3 V, vec3 nL, vec3 albedo, vec3 F0, float metallic, float roughness) {

  vec3 H = normalize(V + nL);
  float NDF = DistributionGGX(N, H, roughness);
  float G = GeometrySmith(N, V, nL, roughness);
  vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

  vec3 numerator = NDF * G * F;
  float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, nL), 0.0) + 0.0001;
  vec3 specular = numerator / denominator;

  vec3 kS = F;
  vec3 kD = vec3(1.0) - kS;
  kD *= 1.0 - metallic;
  float NdotL = max(dot(N, nL), 0.0);

  return (kD * albedo / PI + specular) * NdotL;
}

// ----------------------------------------------------------------------------

vec3 CalcSpotLightPBR(int i, vec3 N, vec3 V, vec3 albedo, vec3 F0, float metallic, float roughness) {

  vec3 L = spotLights[i].position - WorldPos;
  vec3 nL = normalize(L);

  int index = spotLights[i].light_index;
  int attIndex = spotLights[i].attenuation_index;

  float distance = length(L);
  float attenuation = 1.0 / (distance * distance);
  // float attenuation = CalcAttenuation(attIndex, L);

  vec3 radiance = lights[index].ambient * attenuation;

  float intensity = SpotIntensity(i, nL);
  vec3 light = MainPatternPBR(N, V, nL, albedo, F0, metallic, roughness);

  return light * radiance * intensity;
}

// ----------------------------------------------------------------------------

vec3 CalcPointLightPBR(int i, vec3 N, vec3 V, vec3 albedo, vec3 F0, float metallic, float roughness) {
  vec3 L = pointLights[i].position - WorldPos;
  vec3 nL = normalize(L);

  int index = pointLights[i].light_index;
  int attIndex = pointLights[i].attenuation_index;

  float distance = length(L);
  float attenuation = 1.0 / (distance * distance);
  // float attenuation = CalcAttenuation(attIndex, L);

  vec3 radiance = lights[index].ambient * attenuation;
  vec3 light = MainPatternPBR(N, V, nL, albedo, F0, metallic, roughness);

  return light * radiance;
}

// ----------------------------------------------------------------------------

vec3 CalcDirectLightPBR(int i, vec3 N, vec3 V, vec3 albedo, vec3 F0, float metallic, float roughness) {
  vec3 nL = normalize(-dirLights[i].direction);
  vec3 light = MainPatternPBR(N, V, nL, albedo, F0, metallic, roughness);
  return light;
}

// ----------------------------------------------------------------------------

vec3 CalcLightPBR(vec3 N, vec3 V, vec3 albedo, vec3 F0, float metallic, float roughness) {
  vec3 light;
  for(int i = 0; i < dirLightCount; ++i) light += CalcDirectLightPBR(i, N, V, albedo, F0, metallic, roughness);
  for(int i = 0; i < pointLightCount; ++i) light += CalcPointLightPBR(i, N, V, albedo, F0, metallic, roughness);
  for(int i = 0; i < spotLightCount; ++i) light += CalcSpotLightPBR(i, N, V, albedo, F0, metallic, roughness);

  return light;
}
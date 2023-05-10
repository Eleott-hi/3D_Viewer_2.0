#version 410 core

const float PI = 3.14159265359;

out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D albedoMap;
    sampler2D specularMap;
    sampler2D normalMap;
    sampler2D roughnessMap;
    sampler2D aoMap;

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

uniform vec3 camPos;
uniform Light lights[60];
uniform Material material;
uniform DirLight dirLights[20];
uniform SpotLight spotLights[20];
uniform PointLight pointLights[20];
uniform Attenuation attenuations[60];

uniform int dirLightCount;
uniform int pointLightCount;
uniform int spotLightCount;

vec3 CalcSpotLight(int i, vec3 N, vec3 V, vec3 albedo, vec3 F0, float metallic, float roughness);
vec3 CalcPointLight(int i, vec3 N, vec3 V, vec3 albedo, vec3 F0, float metallic, float roughness);
vec3 CalcDirectLight(int i, vec3 N, vec3 V, vec3 albedo, vec3 F0, float metallic, float roughness);
vec3 getNormalFromMap();
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main() {
    vec3 albedo = pow(texture(material.albedoMap, TexCoords).rgb, vec3(2.2));
    float metallic = texture(material.specularMap, TexCoords).r;
    float roughness = texture(material.roughnessMap, TexCoords).r;
    float ao = texture(material.aoMap, TexCoords).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - WorldPos);
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 Lo;

    for(int i = 0; i < dirLightCount; ++i) Lo += CalcDirectLight(i, N, V, albedo, F0, metallic, roughness);
    for(int i = 0; i < pointLightCount; ++i) Lo += CalcPointLight(i, N, V, albedo, F0, metallic, roughness);
    for(int i = 0; i < spotLightCount; ++i) Lo += CalcSpotLight(i, N, V, albedo, F0, metallic, roughness);

    vec3 ambient = vec3(0.01) * albedo * ao;
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}

vec3 CalcSpotLight(int i, vec3 N, vec3 V, vec3 albedo, vec3 F0, float metallic, float roughness) {

    vec3 L_tmp = spotLights[i].position - WorldPos;
    vec3 L = normalize(L_tmp);
    vec3 H = normalize(V + L);

    float distance = length(L_tmp);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lights[pointLights[i].light_index].ambient * attenuation;

    float theta = dot(L, normalize(-spotLights[i].direction));
    float epsilon = spotLights[i].inner_cone - spotLights[i].outer_cone;
    float intensity = clamp((theta - spotLights[i].outer_cone) / epsilon, 0.0, 1.0);

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = max(dot(N, L), 0.0);

    return (kD * albedo / PI + specular) * radiance * NdotL * intensity;
}

vec3 CalcPointLight(int i, vec3 N, vec3 V, vec3 albedo, vec3 F0, float metallic, float roughness) {

    vec3 L_tmp = pointLights[i].position - WorldPos;
    vec3 L = normalize(L_tmp);
    vec3 H = normalize(V + L);

    float distance = length(L_tmp);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lights[pointLights[i].light_index].ambient * attenuation;

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = max(dot(N, L), 0.0);

    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcDirectLight(int i, vec3 N, vec3 V, vec3 albedo, vec3 F0, float metallic, float roughness) {

    vec3 L = normalize(-dirLights[i].direction);
    vec3 H = normalize(V + L);

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = max(dot(N, L), 0.0);

    return (kD * albedo / PI + specular) * NdotL;

    // vec3 lightDir = normalize(-dirLights[i].direction);
    // vec3 reflectDir = reflect(-lightDir, N);

    // float diff = max(dot(N, lightDir), 0.0);
    // float spec = pow(max(dot(V, reflectDir), 0.0), material.shininess);

    // vec3 ambient = lights[dirLights[i].light_index].ambient * vec3(texture(material.albedoMap, TexCoords));
    // vec3 diffuse = lights[dirLights[i].light_index].diffuse * diff * vec3(texture(material.albedoMap, TexCoords));
    // vec3 specular = lights[dirLights[i].light_index].specular * spec * vec3(texture(material.specularMap, TexCoords));

    // return (ambient + diffuse + specular);
}

// ----------------------------------------------------------------------------

vec3 getNormalFromMap() {
    vec3 tangentNormal = texture(material.normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(WorldPos);
    vec3 Q2 = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N = normalize(Normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

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

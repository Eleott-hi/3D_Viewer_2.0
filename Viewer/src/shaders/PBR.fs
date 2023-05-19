#version 410 core

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

uniform vec3 camPos;
uniform Material material;
uniform samplerCube irradianceMap;

vec3 getNormalFromMap();

vec3 CalcLightPBR(vec3 N, vec3 V, vec3 albedo, vec3 F0, float metallic, float roughness);

//! import include/light.glsl

void main() {
    vec3 albedo = pow(texture(material.albedoMap, TexCoords).rgb, vec3(2.2));
    float metallic = texture(material.specularMap, TexCoords).r;
    float roughness = texture(material.roughnessMap, TexCoords).r;
    float ao = texture(material.aoMap, TexCoords).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - WorldPos);
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 Lo = CalcLightPBR(N, V, albedo, F0, metallic, roughness);

    // vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
    // vec3 kD = 1.0 - kS;
    // kD *= 1.0 - metallic;
    // vec3 irradiance = texture(irradianceMap, N).rgb;
    // vec3 diffuse = irradiance * albedo;
    // vec3 ambient = (kD * diffuse) * ao;

    vec3 ambient = vec3(0.01) * albedo * ao;

    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}

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

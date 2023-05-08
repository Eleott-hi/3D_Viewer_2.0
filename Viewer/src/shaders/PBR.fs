#version 410 core
out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;

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

uniform vec3 viewPos;
uniform Material material;
uniform Light lights[60];
uniform DirLight dirLights[20];
uniform SpotLight spotLights[20];
uniform PointLight pointLights[20];
uniform Attenuation attenuations[60];

uniform int dirLightCount;
uniform int pointLightCount;
uniform int spotLightCount;

vec3 getNormalFromMap();
vec3 CalcSpotLight(int i, vec3 N, vec3 V);
vec3 CalcPointLight(int i, vec3 N, vec3 V);
vec3 CalcDirectLight(int i, vec3 N, vec3 V);

void main() {
    vec3 N = getNormalFromMap();
    vec3 V = normalize(viewPos - WorldPos);
    vec3 light;

    for(int i = 0; i < spotLightCount; i++) light += CalcSpotLight(i, N, V);
    for(int i = 0; i < dirLightCount; i++) light += CalcDirectLight(i, N, V);
    for(int i = 0; i < pointLightCount; i++) light += CalcPointLight(i, N, V);

    FragColor = vec4(light, 1.0);
}

vec3 CalcSpotLight(int i, vec3 N, vec3 V) {
    vec3 tmp = spotLights[i].position - WorldPos;
    vec3 lightDir = normalize(tmp);
    vec3 reflectDir = reflect(-lightDir, N);

    float diff = max(dot(N, lightDir), 0.0);
    float spec = pow(max(dot(V, reflectDir), 0.0), material.shininess);

    float theta = dot(lightDir, normalize(-spotLights[i].direction));
    float epsilon = spotLights[i].inner_cone - spotLights[i].outer_cone;
    float intensity = clamp((theta - spotLights[i].outer_cone) / epsilon, 0.0, 1.0);

    vec3 ambient = lights[spotLights[i].light_index].ambient * vec3(texture(material.diffuseMap, TexCoords));
    vec3 diffuse = lights[spotLights[i].light_index].diffuse * diff * vec3(texture(material.diffuseMap, TexCoords));
    vec3 specular = lights[spotLights[i].light_index].specular * spec * vec3(texture(material.specularMap, TexCoords));

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

vec3 CalcPointLight(int i, vec3 N, vec3 V) {
    vec3 lightDir = pointLights[i].position - WorldPos;
    vec3 nlightDir = normalize(lightDir);
    vec3 reflectDir = reflect(-nlightDir, N);

    float diff = max(dot(N, nlightDir), 0.0);
    float spec = pow(max(dot(V, reflectDir), 0.0), material.shininess);

    vec3 ambient = lights[pointLights[i].light_index].ambient * vec3(texture(material.diffuseMap, TexCoords));
    vec3 diffuse = lights[pointLights[i].light_index].diffuse * diff * vec3(texture(material.diffuseMap, TexCoords));
    vec3 specular = lights[pointLights[i].light_index].specular * spec * vec3(texture(material.specularMap, TexCoords));

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

vec3 CalcDirectLight(int i, vec3 N, vec3 V) {
    vec3 lightDir = normalize(-dirLights[i].direction);
    vec3 reflectDir = reflect(-lightDir, N);

    float diff = max(dot(N, lightDir), 0.0);
    float spec = pow(max(dot(V, reflectDir), 0.0), material.shininess);

    vec3 ambient = lights[dirLights[i].light_index].ambient * vec3(texture(material.diffuseMap, TexCoords));
    vec3 diffuse = lights[dirLights[i].light_index].diffuse * diff * vec3(texture(material.diffuseMap, TexCoords));
    vec3 specular = lights[dirLights[i].light_index].specular * spec * vec3(texture(material.specularMap, TexCoords));

    return (ambient + diffuse + specular);
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

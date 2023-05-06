#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

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

uniform Attenuation u_attenuations[60];
uniform PointLight u_pointLights[20];
uniform SpotLight u_spotLights[20];
uniform DirLight u_dirLights[20];
uniform Light u_lights[60];


uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

uniform vec3 lightPos = vec3(0,0,0);
uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
        
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}
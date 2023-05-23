#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

uniform bool reverse_normals = false;

void main() {
    vs_out.FragPos = vec3(Model * vec4(aPos, 1.0));
    if(reverse_normals) // a slight hack to make sure the outer large cube displays lighting from the 'inside' instead of the default 'outside'.
        vs_out.Normal = transpose(inverse(mat3(Model))) * (-1.0 * aNormal);
    else
        vs_out.Normal = transpose(inverse(mat3(Model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    gl_Position = Projection * View * vec4(vs_out.FragPos, 1.0);
}
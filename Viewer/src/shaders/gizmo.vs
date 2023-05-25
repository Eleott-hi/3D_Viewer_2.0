#version 410

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;

out vec4 v_color;
out vec3 v_world, v_normal;

uniform mat4 u_mvp;

void main() {
    gl_Position = u_mvp * vec4(vertex.xyz, 1);
    v_color = color;
    v_world = vertex;
    v_normal = normalize(normal);
}
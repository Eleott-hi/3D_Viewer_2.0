#version 410

in vec4 v_color;
in vec3 v_world, v_normal;

out vec4 f_color;

uniform vec3 u_eye;

void main() {
    vec3 light = vec3(1) * max(dot(v_normal, normalize(u_eye - v_world)), 0.50) + 0.25;
    f_color = v_color * vec4(light, 1);

    f_color = v_color;
}
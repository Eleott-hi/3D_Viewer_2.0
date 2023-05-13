#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main() {
    FragColor = texture(screenTexture, TexCoords);

    // vec3 FragPos = texture(gPosition, TexCoords).rgb;
    // vec3 Normal = texture(gNormal, TexCoords).rgb;
    // vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    // float Specular = texture(gAlbedoSpec, TexCoords).a;

    // FragColor = vec4(Normal, 1);
}
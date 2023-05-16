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

// void main()
// {             
//     float depthValue = texture(screenTexture, TexCoords).r;
//     // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
//     FragColor = vec4(vec3(depthValue), 1.0); // orthographic
// }
#version  410 core

// out vec4 FragColor;

void main()
{          
    // FragColor =    vec4(vec3(gl_FragCoord.z), 1.0);
    gl_FragDepth = 0.5;
}
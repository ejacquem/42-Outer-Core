#version 330 core

in vec3 ourColor;

out vec4 FragColor;  // Output color

void main() {
    // Use the Primitive ID to alternate colors
    int modid = gl_PrimitiveID % 6;
    if (modid == 0)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else if (modid == 1)
        FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    else if (modid == 2)
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    else if (modid == 3)
        FragColor = vec4(0.0, 1.0, 1.0, 1.0);
    else if (modid == 4)
        FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    else
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
#version 330 core

in vec3 ourColor;

out vec4 FragColor;  // Output color

vec3 red = vec3(1.0, 0.0, 0.0);
vec3 green = vec3(0.0, 1.0, 0.0);
vec3 blue = vec3(0.0, 0.0, 1.0);
vec3 yellow = vec3(1.0, 1.0, 0.0);
vec3 cyan = vec3(0.0, 1.0, 1.0);
vec3 magenta = vec3(1.0, 0.0, 1.0);

void main() {
    // Use the Primitive ID to alternate colors
    int modid = gl_PrimitiveID % 6;
    if (modid == 0)
        FragColor = vec4(red, 1.0);
    else if (modid == 1)
        FragColor = vec4(yellow, 1.0);
    else if (modid == 2)
        FragColor = vec4(green, 1.0);
    else if (modid == 3)
        FragColor = vec4(cyan, 1.0);
    else if (modid == 4)
        FragColor = vec4(blue, 1.0);
    else if (modid == 5)
        FragColor = vec4(magenta, 1.0);
}
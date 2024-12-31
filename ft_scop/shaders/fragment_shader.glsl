#version 330 core

in vec3 color;

out vec4 FragColor;  // Output color

void main() {
    FragColor = vec4(color, 1.0);  // Set color

}

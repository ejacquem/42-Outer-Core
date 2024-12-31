#version 330 core
layout (location = 0) in vec3 aPos;  // Position input
layout (location = 1) in vec3 aColor;  // Color input

out vec3 color;

uniform float xOffset;
uniform float xOffset2;
uniform float yOffset;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);  // Transform vertex position
    if(aPos.x < 0)
        gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0);  // Transform vertex position
    if(aPos.x > 0)
        gl_Position = vec4(aPos.x + xOffset2, aPos.y, aPos.z, 1.0);  // Transform vertex position
    if(aPos.y > 0)
        gl_Position = vec4(aPos.x, aPos.y + yOffset, aPos.z, 1.0);  // Transform vertex position
    color = aColor;
}
#version 330 core

layout(location = 0) in vec3 aPos;    // Position
layout(location = 1) in vec2 aTexCoord; // UV coordinates

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = vec3(1.0f);
    vec3 absPos = abs(aPos);
    // if (absPos.x > absPos.y && absPos.x > absPos.z) {
    //     // X face
    //     TexCoord.x = 0.5 * (aPos.z / absPos.x + 1.0);
    //     TexCoord.y = 0.5 * (aPos.y / absPos.x + 1.0);
    // } else if (absPos.y > absPos.x && absPos.y > absPos.z) {
    //     // Y face
    //     TexCoord.x = 0.5 * (aPos.x / absPos.y + 1.0);
    //     TexCoord.y = 0.5 * (aPos.z / absPos.y + 1.0);
    // } else {
    //     // Z face
    //     TexCoord.x = 0.5 * (aPos.x / absPos.z + 1.0);
    //     TexCoord.y = 0.5 * (aPos.y / absPos.z + 1.0);
    // }

    TexCoord = aTexCoord;
}
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float xOffset;
uniform float xOffset2;
uniform float yOffset;
uniform float yOffset2;

void main()
{
    vec4 pos = vec4(aPos, 1.0);
    if(aPos.x < 0)
    {
        if(aPos.y > 0)
            pos.y += yOffset;
        else
            pos.x += xOffset;
    }
    if(aPos.x > 0)
    {
        if(aPos.y > 0)
            pos.y += yOffset2;
        else
            pos.x += xOffset2;
    }
    gl_Position = pos;
    ourColor = aColor;
    TexCoord = aTexCoord;
}
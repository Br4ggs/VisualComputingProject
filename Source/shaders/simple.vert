#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec2 iTexCoord;

out vec2 oTexCoord;

void main()
{
   oTexCoord = iTexCoord;
   gl_Position = vec4(iPos, 1.0);
}

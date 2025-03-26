#version 330

in vec3 vCol;
in vec3 vPos;

out vec3 color;

void main()
{
    gl_Position = vec4(vPos, 1.0);
    color = vCol;
}

#version 330

in Data
{
	vec2 texCoords;
} DataIn;

uniform sampler2D tex;

out vec4 fragColor;

void main()
{
	fragColor = texture(tex, DataIn.texCoords);
}
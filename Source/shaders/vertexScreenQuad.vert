#version 330

layout (location = 0) in vec3 pos;

//texture coordinate output
out Data {
	vec2 texCoords;
} DataOut;

//maps incoming vertex data for a unit-quad to the screen
void main()
{
	gl_Position = vec4(pos.xy, 0.0, 1.0);

	DataOut.texCoords = gl_Position.xy * 0.5 + 0.5f;
}
#ifndef SHADER_PROG_H
#define SHADER_PROG_H

#include <filesystem>
#include <glad/glad.h>

//TODO:
//-deconstructor
class ShaderProgram
{
public:
	int attachVertexShader(const char* sourceFile);
	int attachFragmentShader(const char* sourceFile);
	int compile();
	void use();
	void destroy();

private:
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint shaderProgram = 0;
};

#endif // !SHADER_PROG_H

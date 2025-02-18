#ifndef SHADER_PROG_H
#define SHADER_PROG_H

#include <filesystem>
#include <glad/glad.h>

//Wrapper class for opengl shader programs
//Usage:
//1. attach a vertex and fragment shader
//2. compile the shader program
//3. before rendering make sure to call use()
//4. when you are done using the program call destroy()
class ShaderProgram
{
public:
	ShaderProgram() = default;
	~ShaderProgram();

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

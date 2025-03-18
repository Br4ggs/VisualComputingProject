#include "header/shaderProgram.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::~ShaderProgram()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
}

int ShaderProgram::attachVertexShader(const char* sourceFile)
{
	std::ifstream in(sourceFile);
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	const char* source = contents.c_str();

	std::cout << "Found shader:" << std::endl;
	std::cout << source << std::endl;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &source, NULL);

	glCompileShader(vertexShader);

	int success;
	char info[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		std::cout << "Error compiling vertex shader:\n" << info << std::endl;
		return -1;
	}
	else
	{
		std::cout << "Vertex shader compiled succesfully" << std::endl;
		return 0;
	}
}

int ShaderProgram::attachFragmentShader(const char* sourceFile)
{
	std::ifstream in(sourceFile);
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	const char* source = contents.c_str();

	std::cout << "Found shader:" << std::endl;
	std::cout << source << std::endl;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &source, NULL);

	glCompileShader(fragmentShader);

	int success;
	char info[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		std::cout << "Error compiling fragment shader:\n" << info << std::endl;
		return -1;
	}
	else
	{
		std::cout << "Fragment shader compiled succesfully" << std::endl;
		return 0;
	}
}

int ShaderProgram::compile()
{
	if (vertexShader == 0 || fragmentShader == 0) return -1;

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	int success;
	char info[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, info);
		std::cout << "Error compiling shader program:\n" << info << std::endl;
		return -1;
	}
	else
	{
		std::cout << "Shader program linked succesfully" << std::endl;

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		vertexShader = 0;
		fragmentShader = 0;

		return 0;
	}
}

unsigned int ShaderProgram::getRawShaderProgram()
{
	return shaderProgram;
}

void ShaderProgram::use() const
{
	glUseProgram(shaderProgram);
}

void ShaderProgram::passUniform4x4floatMatrix(char* name, glm::mat4 matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::passUniform3floatVector(char* name, glm::vec3 vector) const
{
	glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, glm::value_ptr(vector));
}

void ShaderProgram::destroy()
{
	glDeleteProgram(shaderProgram);
}

#ifndef MESHOBJ_H
#define MESHOBJ_H

#include "shaderProgram.h"

#include <glad/glad.h>

//Simple class that represents a mesh object rendered using the provided shader program
class TexturedScreenQuad
{
public:
	TexturedScreenQuad(const ShaderProgram* program);
	//~TexturedScreenQuad();
	void render() const;
	void destroy();

private:
	const ShaderProgram* program;
	
	//vertex data in normalized device coordinates
	//defines a quad as follows:
	// 1---2
	// | / |
	// 3---4
	GLfloat vertices[12] =
	{
		-1.0f, 1.0f, 0.0f, //x, y, z
		1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};

	GLuint VAO;
	GLuint VBO;
};

#endif // !MESHOBJ_H

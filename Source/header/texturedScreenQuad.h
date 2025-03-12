#ifndef MESHOBJ_H
#define MESHOBJ_H

#include "shaderProgram.h"

#include <glad/glad.h>

//Represents a textured quad stretched over the entire viewport
//intended for visualizing visual data on screen using opengl
class TexturedScreenQuad
{
public:
	TexturedScreenQuad(const ShaderProgram* program);
	//~TexturedScreenQuad();

	//Writes provided data to opengl texture
	//data must be provided in RGB format
	int writeToTexture(const unsigned int width, const unsigned int height, const unsigned char* data);
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
	GLuint texture;
};

#endif // !MESHOBJ_H

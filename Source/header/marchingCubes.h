//credits:  originally authored by kishan on 19/02/2025
//			edited by by emiel

#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include <vector>
#include <glm/glm.hpp>

#include "scene.h"
#include "shaderProgram.h"

class MarchingCubes
{
public:
	MarchingCubes(int displayWidth, int displayHeight, Scene* scene, ShaderProgram* shaderProg);

	void drawUI();
	void render() const;

	void regenerateMarchingCubes();


private:
	// point on the marching cubes grid consisting of a position and its value
	struct GridPoint
	{
		glm::vec3 position;
		float value;
	};

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		//TODO: color
	};

	Scene* scene;
	ShaderProgram* shaderProg;

	int displayWidth;
	int displayHeight;

	GLuint VAO;
	GLuint VBO;

	//grid
	int gridSize = 30;
	float gridSpacing = 0.1f;

	std::vector<std::vector<std::vector<GridPoint>>> grid;

	std::vector<Vertex> vertices;


	void marchingCubes();
	glm::vec3 interpolate(GridPoint p1, GridPoint p2) const;
	glm::vec3 getNormal(glm::vec3 point) const;
};

#endif

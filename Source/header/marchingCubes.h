//credits:  originally authored by kishan on 19/02/2025
//			edited by by emiel

#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include <vector>
#include <glm/glm.hpp>

#include "scene.h"

class MarchingCubes
{
public:
	MarchingCubes();

	void drawUI();

	void regenerateMarchingCubes(Scene* scene);


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
	};

	Scene* currentScene;

	//grid
	const int gridSize = 30;
	const float gridSpacing = 0.1f;

	std::vector<std::vector<std::vector<GridPoint>>> grid;

	std::vector<Vertex> vertices;


	void marchingCubes();
	glm::vec3 interpolate(GridPoint p1, GridPoint p2) const;
	glm::vec3 getNormal(glm::vec3 point) const;
};

#endif

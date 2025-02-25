#ifndef VECUTILS_H
#define VECUTILS_H

#include <glm/glm.hpp>

//selects the largest components from a vec3
inline float vmax(glm::vec3 v)
{
	return glm::max(glm::max(v.x, v.y), v.z);
}

//selects the smallest components from a vec3
inline float vmin(glm::vec3 v)
{
	return glm::min(glm::min(v.x, v.y), v.z);
}

#endif // !VECUTILS_H

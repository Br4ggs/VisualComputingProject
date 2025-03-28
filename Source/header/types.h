#ifndef _TYPES_H
#define _TYPES_H

#include <cstdint>
#include <glm/glm.hpp>
#include <stdexcept>
#include <variant>
#include <glad/glad.h>


class Scene;

// NOTE: should be the same as the shader definition
typedef struct LinearCSGTreeNode {
	int32_t op;
	int32_t shape;
	float _padding[2];
	glm::vec4 metadata1;  /* see notes below */
	glm::vec4 dimensions;
	glm::mat4 transform;
	glm::vec4 scale;
	glm::vec4 color;
} LinearCSGTreeNode;
/*NOTE: metadata1[0] has smoothing factor for smooth min */

// NOTE: should be the same as the shader constants
enum CSGOperation {
	OP_UNI = 0,
	OP_INT = 1,
	OP_DIFF = 2,
	OP_MOD = 3,
	OP_SMUN = 4,
	NO_OP = 5,
};

// NOTE: should be the same as the shader constants
enum CSGShape {
	SHAPE_SPHERE = 0,
	SHAPE_BOX = 1,
	SHAPE_CYL = 2,
	SHAPE_PLANE = 3,
	NO_SHAPE = 4,
};

using CSGType = std::variant<CSGShape, CSGOperation>;

namespace RenderStrategy {
enum class Type {
	SPHERE_MARCHING_CPU = 0,
	CUBE_MARCHING_CPU = 1,
	SPHERE_MARCHING_GPU = 2,

	COUNT /* HACK: amount of types */
};

constexpr const char* toString(Type type) {
	switch (type) {
	case Type::SPHERE_MARCHING_CPU:
		return "Sphere Marching (CPU)";
	case Type::CUBE_MARCHING_CPU:
		return "Cube Marching (CPU)";
	case Type::SPHERE_MARCHING_GPU:
		return "Sphere Marching (GPU)";
	default:
		throw std::logic_error("Invalid render strategy");
	}
}

}
#endif

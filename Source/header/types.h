#ifndef _TYPES_H
#define _TYPES_H

#include <cstdint>
#include <glm/glm.hpp>
#include <variant>

typedef struct LinearCSGTreeNode {
	int32_t op;
	int32_t shape;
	float _padding[2];
	glm::vec4 position;
	glm::vec4 dimensions;
} LinearCSGTreeNode;

enum CSGOperation {
	OP_UNI = 0,
	OP_INT = 1,
	OP_DIFF = 2,
	OP_MOD = 3,
	NO_OP = 4,
};

enum CSGShape {
	SHAPE_SPHERE = 0,
	SHAPE_BOX = 1,
	SHAPE_CYL = 2,
	SHAPE_PLANE = 3,
	NO_SHAPE = 4,
};

using CSGType = std::variant<CSGShape, CSGOperation>;


#endif

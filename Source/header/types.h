#ifndef _TYPES_H
#define _TYPES_H

#include <cstdint>
#include <glm/glm.hpp>
#include <stdexcept>
#include <variant>

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

namespace RenderType {

enum Type {
	SPHERE_MARCHING_CPU = 0,
	CUBE_MARCHING_CPU = 1,
	SPHERE_MARCHING_GPU = 2,

	COUNT /* HACK: amount of types */
};

static const char* names[] = {
	"Sphere Marching (CPU)",
	"Cube Marching (CPU)",
	"Sphere Marching (GPU)",
};

inline const char* getName(Type type) {

	if (type >= 0 && type <= COUNT) {
		return names[type];
	}

	throw std::invalid_argument(&"That render type does not exist: " [ type]);
}

inline const Type getType(int type) {
	// should return the correct Type based on the index
	if (type >= 0 && type < COUNT) {
		return static_cast<Type>(type);
	}
	throw std::invalid_argument(&"That render type does not exist: " [ type]);
}

inline void allNames(const char *** dst) {
	*dst = names;
}

}

#endif

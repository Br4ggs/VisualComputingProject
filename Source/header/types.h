#ifndef _TYPES_H
#define _TYPES_H

#include <cstdint>
#include <glm/glm.hpp>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <variant>
#include <glad/glad.h>


class Scene; // forward declaration

// NOTE: the struct and enums should be the same as the shader definition
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

enum CSGOperation {
    OP_UNI = 0,
    OP_INT = 1,
    OP_DIFF = 2,
    OP_MOD = 3,
    OP_SMUN = 4,
    NO_OP = 5, /* HACK: keep this as last item for the operation count */
};
// NOTE: order should be the same as the enum
constexpr const char* operationNames[] = {
    "Union",
    "Intersection",
    "Difference",
    "Modulus",
    "Smooth Union"
};

enum CSGShape {
    SHAPE_SPHERE = 0,
    SHAPE_BOX = 1,
    SHAPE_CYL = 2,
    SHAPE_PLANE = 3,
    NO_SHAPE = 4, /* HACK: keep this as last item for the shape count */
};
// NOTE: order should be the same as the enum
constexpr const char* shapeNames[] = {
    "Sphere",
    "Box",
    "Cyllinder",
    "Plane"
};

using CSGType = std::variant<CSGShape, CSGOperation>;

inline std::optional<CSGType> parseCSGType(const std::string_view input) {
    for (int i = 0; i < NO_SHAPE; ++i) {
        if (strcasecmp(input.data(), shapeNames[i]) == 0) {
            return CSGType(static_cast<CSGShape>(i));
        }
    }
    for (int i = 0; i < NO_OP; ++i) {
        if (strcasecmp(input.data(), operationNames[i]) == 0) {
            return CSGType(static_cast<CSGOperation>(i));
        }
    }
    return std::nullopt;
}


struct CSGVisitor {
    const char* operator()(CSGShape shape) const {
        switch (shape) {
            case SHAPE_SPHERE:
                return "Sphere";
            case SHAPE_BOX:
                return "Box";
            case SHAPE_CYL:
                return "Cylinder";
            case SHAPE_PLANE:
                return "Plane";
            case NO_SHAPE:
                return "No Shape";
        }
        throw std::logic_error("Unknown shape");
    }

    const char* operator()(CSGOperation op) const {
        switch (op) {
            case OP_UNI:
                return "Union";
            case OP_INT:
                return "Intersection";
            case OP_DIFF:
                return "Difference";
            case OP_MOD:
                return "Modulus";
            case OP_SMUN:
                return "Smooth Union";
            case NO_OP:
                return "No Operation";
        }
        throw std::logic_error("Unknown operation");
    }
};

constexpr const char* csgTypeToString(const CSGType& type) {
    return std::visit(CSGVisitor(), type);
}


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

#include <string_view>
#include <array>
#include <stdexcept>

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

	throw std::invalid_argument("That render type does not exist");
}

inline void allNames(const char *** dst) {
	*dst = names;
}

}

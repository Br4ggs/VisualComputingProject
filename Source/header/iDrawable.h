#ifndef IDRAWABLE_H
#define IDRAWABLE_H

#include <glm/glm.hpp>
#include <utility>

class IDrawable
{
public:
	virtual ~IDrawable() {}
	virtual void drawUI() = 0;
	virtual std::pair<float, glm::vec3> sdf(glm::vec3 point) const = 0;
};

#endif // !IDRAWABLE_H

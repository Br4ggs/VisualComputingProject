#ifndef IDRAWABLE_H
#define IDRAWABLE_H

#include <glm/glm.hpp>

class IDrawable
{
public:
	virtual ~IDrawable() {}
	virtual void drawUI() = 0;
	virtual glm::vec2 sdf(glm::vec3 point) const = 0;
};

#endif // !IDRAWABLE_H

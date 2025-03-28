#ifndef IDRAWABLE_H
#define IDRAWABLE_H

#include <glm/glm.hpp>
#include <utility>
#include <vector>

#include "types.h"

class IDrawable
{
public:
	virtual ~IDrawable() {}
	virtual void drawUI(bool& dirty) = 0;

	const char* getName() const { return csgTypeToString(type); };
	const CSGType getType() const { return type; };

	virtual std::vector<IDrawable*> getChildren() const = 0;
	virtual std::vector<IDrawable*> detachChildren() = 0;

	virtual std::pair<float, glm::vec3> sdf(glm::vec3 point) const = 0;

protected:
	CSGType type;
};

#endif

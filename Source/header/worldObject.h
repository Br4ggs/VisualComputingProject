#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "iDrawable.h"

//represents an objects that has a transform in space
class WorldObject : public IDrawable
{
public:
	virtual ~WorldObject() {};

	void setPosition(glm::vec3 pos);
	void setRotation(glm::mat4 rot);
	void setScale(glm::vec3 scl);

protected:
	WorldObject();

	void recalculateTransform();

	//rotation?

	glm::mat4 position;
	glm::mat4 rotation;
	glm::vec3 scale;

	glm::mat4 transform;
};

#endif // !WORLDOBJECT_H

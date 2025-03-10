#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "iDrawable.h"

//represents an objects that has a transform and color in space
class WorldObject : public IDrawable
{
public:
	virtual ~WorldObject() {};

	virtual std::vector<IDrawable*> getChildren() const;
	virtual std::vector<IDrawable*> detachChildren();

	void setPosition(glm::vec3 pos);
	void setRotation(glm::mat4 rot);
	void setScale(glm::vec3 scl);

protected:
	WorldObject();

	void recalculateTransform();

	glm::vec3 color;

	glm::mat4 position;
	glm::mat4 rotation;
	glm::vec3 scale;

	glm::mat4 transform;
};

#endif // !WORLDOBJECT_H

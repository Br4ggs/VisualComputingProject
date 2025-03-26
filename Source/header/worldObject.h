#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "iDrawable.h"

//represents an objects that has a transform and color in space
class WorldObject : public IDrawable
{
public:
	~WorldObject() override {};

	void drawUI(bool& dirty) override;

	std::vector<IDrawable*> getChildren() const override;
	std::vector<IDrawable*> detachChildren() override;

	void setPosition(glm::vec3 pos);
	void setRotation(glm::mat4 rot);
	void setScale(glm::vec3 scl);

	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::mat4 getTransform();
	glm::vec3 getCol();

protected:
	float posf[3] = { 0.0f, 0.0f, 0.0f };
	float rotf[3] = { 0.0f, 0.0f, 0.0f };
	float sclf[3] = { 1.0f, 1.0f, 1.0f };

	glm::vec3 color;

	glm::mat4 position;
	glm::mat4 rotation;
	glm::vec3 scale;

	glm::mat4 transform;

	WorldObject();
	void recalculateTransform();
};

#endif

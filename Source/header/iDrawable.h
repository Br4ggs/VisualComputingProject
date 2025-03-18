#ifndef IDRAWABLE_H
#define IDRAWABLE_H

#include <glm/glm.hpp>
#include <utility>
#include <vector>
#include <string>

class IDrawable
{
public:
	virtual ~IDrawable() {}
	virtual void drawUI() = 0;

	virtual char* getName() const = 0;

	/* HACK: op and shape need to be distinguished in the tree */
	virtual bool isOp() = 0;
	virtual bool isShape() = 0;

	/// <summary>
	/// todo
	/// </summary>
	/// <returns></returns>
	virtual std::vector<IDrawable*> getChildren() const = 0;

	//TODO: kind of an ugly solution: this is only really used by operators, so maybe we could alternatively try upcasting
	//		an IDrawable to an IOperator child class?
	/// <summary>
	/// todo
	/// caller takes on ownership of child objects!
	/// </summary>
	/// <returns></returns>
	virtual std::vector<IDrawable*> detachChildren() = 0;

	virtual std::pair<float, glm::vec3> sdf(glm::vec3 point) const = 0;
};

#endif

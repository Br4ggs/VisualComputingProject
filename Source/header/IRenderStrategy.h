#ifndef IRENDERSTRATEGY_H
#define IRENDERSTRATEGY_H

#include "types.h"

class IRenderStrategy 
{
public:
	IRenderStrategy(RenderStrategyType type);
	~IRenderStrategy();

	virtual void render() = 0;
	virtual void switchedStrategy() = 0;
	virtual void setup() = 0;
	virtual void drawUI() = 0;
	virtual void dirtyUpdate() = 0;

private:
	RenderStrategyType type;
};

#endif

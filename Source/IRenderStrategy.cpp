#include "IRenderStrategy.h"
#include <iostream>

IRenderStrategy::IRenderStrategy(RenderStrategyType type)
:type(type)
{
    std::cout << "constructed irender strategy\n";
}

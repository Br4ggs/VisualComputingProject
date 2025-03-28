#include "IRenderStrategy.h"
#include <iostream>

IRenderStrategy::IRenderStrategy(RenderStrategyType type)
:type(type)
{
    std::cout << "constructed render strategy\n";
}

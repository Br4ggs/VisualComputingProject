#include "IRenderStrategy.h"
#include <iostream>

IRenderStrategy::IRenderStrategy(RenderStrategy::Type type)
:type(type)
{
    std::cout << "constructed irender strategy\n";
}

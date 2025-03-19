#include "linearizeScene.h"
#include "worldObject.h"
#include <iostream>
#include <ostream>

void linearizeDrawable(IDrawable* drawable, std::vector<LinearCSGTreeNode>& linearScene)
{
    CSGType type = drawable->getType();

    if (std::holds_alternative<CSGShape>(type)) { /* shape / leaf */
        WorldObject *wObject = static_cast<WorldObject*>(drawable);
        CSGShape shape = std::get<CSGShape>(type);
        switch(shape) {
        case SHAPE_SPHERE:
        case SHAPE_BOX:
        case SHAPE_CYL:
        case SHAPE_PLANE:
            LinearCSGTreeNode node;
            node.op = CSGOperation::NO_OP;
            node.shape = shape;
            node.position = glm::vec4(wObject->getPosition(), 0.0);
            node.dimensions = glm::vec4(wObject->getScale(), 0.0);
            linearScene.push_back(node);
            break;
        case NO_SHAPE:
            throw std::logic_error("world object should always have a shape");
        default:
            break;
        }
        std::cout << "shape: " << shape << std::endl;
    } else if (std::holds_alternative<CSGOperation>(type)) { /* op / internal */
        auto children = drawable->getChildren();
        for (auto child : children) {
            linearizeDrawable(child, linearScene);
        }
        CSGOperation op = std::get<CSGOperation>(type);
        switch(std::get<CSGOperation>(type)) {
        case OP_UNI:
        case OP_INT:
        case OP_DIFF:
        case OP_MOD:
            LinearCSGTreeNode node;
            node.op = op;
            node.shape = CSGShape::NO_SHAPE;
            break;
        case NO_OP:
            throw std::logic_error("op implementation should always have a valid operation");
            break;
        }
        std::cout << "op: " << op << std::endl;
    }
}

void linearizeScene(Scene *scene, std::vector<LinearCSGTreeNode>& linearScene)
{
    std::cout << "linearizing...\n";

    int counter = 0;

    for (IDrawable *object : scene->getObjects()) {
        std::cout << "linearizing object\n";
        counter++;
        linearizeDrawable(object, linearScene);
    }

    for (int i = 0; i < (counter - 1); ++i) {
        std::cout << "adding trailing union\n";
        LinearCSGTreeNode node;
        node.op = CSGOperation::OP_UNI;
        linearScene.push_back(node);
    }

    std::cout << "...done linearizing\n";
}


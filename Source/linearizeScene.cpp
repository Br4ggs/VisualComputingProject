#include "linearizeScene.h"
#include "opSmoothUnion.h"
#include "worldObject.h"
#include <iomanip>
#include <iostream>
#include <ostream>
#include "types.h"

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
            node.metadata1 = glm::vec4(wObject->getPosition(), 1.0);
            node.dimensions = glm::vec4(1.0f);
            node.scale = glm::vec4(wObject->getScale(), 0.0);
            node.transform = wObject->getTransform();
            node.color = glm::vec4(wObject->getColor(), 0.0);
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
        LinearCSGTreeNode node;
        switch(op) {
        case OP_SMUN:
            node.metadata1 = glm::vec4(static_cast<OpSmoothUnion*>(drawable)->getSmoothingFactor() ,1.0,1.0,1.0);
        case OP_UNI:
        case OP_INT:
        case OP_DIFF:
        case OP_MOD:
            node.op = op;
            node.shape = CSGShape::NO_SHAPE;
            linearScene.push_back(node);
            break;
        case NO_OP:
        default:
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
        node.shape = CSGShape::NO_SHAPE;
        linearScene.push_back(node);
    }

    std::cout << "...done linearizing\n";

    for (auto &x : linearScene) 
    {
        printLinearCSGTreeNode(x);
    }
}

void printLinearCSGTreeNode(const LinearCSGTreeNode& node) {
    // Print operation and shape
    std::cout << "CSG Node Details:" << std::endl;

    // Convert integer to operation/shape names using previous visitor
    CSGType opType = static_cast<CSGOperation>(node.op);
    CSGType shapeType = static_cast<CSGShape>(node.shape);

    std::cout << "Operation: " << csgTypeToString(opType) << std::endl;
    std::cout << "Shape:     " << csgTypeToString(shapeType) << std::endl;

    // Print metadata
    std::cout << "\nMetadata:" << std::endl;
    std::cout << "  Metadata1: (" 
        << node.metadata1.x << ", " 
        << node.metadata1.y << ", " 
        << node.metadata1.z << ", " 
        << node.metadata1.w << ")" << std::endl;

    // Print dimensions
    std::cout << "  Dimensions: (" 
        << node.dimensions.x << ", " 
        << node.dimensions.y << ", " 
        << node.dimensions.z << ", " 
        << node.dimensions.w << ")" << std::endl;

    // Print transform matrix (4x4)
    std::cout << "\nTransform Matrix:" << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << "  [" 
            << std::fixed << std::setprecision(4) 
            << node.transform[i][0] << ", "
            << node.transform[i][1] << ", "
            << node.transform[i][2] << ", "
            << node.transform[i][3] << "]" << std::endl;
    }

    // Print scale
    std::cout << "\nScale: (" 
        << node.scale.x << ", " 
        << node.scale.y << ", " 
        << node.scale.z << ", " 
        << node.scale.w << ")" << std::endl;

    // Print color
    std::cout << "Color: (" 
        << node.color.x << ", " 
        << node.color.y << ", " 
        << node.color.z << ", " 
        << node.color.w << ")" << std::endl;
}


#ifndef LINEARIZESCENE_H
#define LINEARIZESCENE_H

#include <glm/glm.hpp>
#include "scene.h"
#include "types.h"

void linearizeScene(Scene *scene, std::vector<LinearCSGTreeNode>& linearScene);
const char* getCSGOperationName(int op);
const char* getCSGShapeName(int shape);
void printLinearCSGTreeNode(const LinearCSGTreeNode& node);

#endif

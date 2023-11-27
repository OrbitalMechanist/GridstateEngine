#pragma once
#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "glm/glm.hpp"

// position and direction
struct TransformComponent {
    float x, y, z;
    glm::ivec2 pos = { 0, 0 };
};

#endif 

#pragma once

#include <glm/glm.hpp>
#include "ecs/Component.h"

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	Transform() : Transform(glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 1,1,1 }) {}
	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) : position(pos), rotation(rot), scale(scl) {}
};

// This component tracks the object's real space coordinates for the purposes of rendering

class RealTransform : public Component {
public:
	Transform transform;
};

#pragma once
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 texCoord;
	glm::uvec4 boneIndices;
	glm::vec4 boneWeights;
};


#pragma once

#include "graphics/Renderer.h"

struct StaticMeshComponent {
	std::string modelName;
	std::string textureName;
	std::string materialName;
	std::string shaderName;

	glm::vec3 posOffset = { 0.0f, 0.0f, 0.0f };
	glm::vec3 rotOffset = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
};
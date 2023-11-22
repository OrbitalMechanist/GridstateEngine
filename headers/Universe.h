#pragma once
#include "graphics/Renderer.h"
#include "entity/EntityManager.h"

#include "components/TransformComponent.h"
#include "components/StaticMeshComponent.h"

class Universe {
private:
	glm::vec3 origin;
	glm::vec2 cellDimensions;
	EntityManager& em;
	Renderer& renderer;
public:
	Universe(glm::vec3 gridOriginWorldPos, glm::vec2 cellSize, Renderer& rend, EntityManager& em);
	void update(float deltaTime);
};
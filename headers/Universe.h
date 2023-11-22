#pragma once
#include "graphics/Renderer.h"
#include "ecs/entity/EntityManager.h"

#include "ecs/components/TransformComponent.h"
#include "ecs/components/StaticMeshComponent.h"

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
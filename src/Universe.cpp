#pragma once
#include "Universe.h"

Universe::Universe(glm::vec3 gridOriginWorldPos, glm::vec2 cellSize, Renderer& rend,
	EntityManager& em) : renderer(rend), em(em)
{
	origin = gridOriginWorldPos;
	cellDimensions = cellSize;
}

void Universe::update(float deltaTime)
{
	for (auto i : em.getEntitiesWithComponent<StaticMeshComponent>()) {
		TransformComponent& trans = em.getComponent<TransformComponent>(i);

		glm::vec3 gameObjWorldPos = origin + glm::vec3{trans.pos.x * cellDimensions.x,
			trans.pos.y * cellDimensions.y, 0};

		StaticMeshComponent& mesh = em.getComponent<StaticMeshComponent>(i);

		renderer.addRenderObject(RenderObject(mesh.modelName, mesh.textureName, mesh.materialName,
			mesh.shaderName, mesh.posOffset + gameObjWorldPos, mesh.rotOffset, mesh.scale, true, true));
	}
}


#pragma once

#include "ecs/entity/EntityManager.h"
#include "ai/AISystem.h"


class AISystemDemoTest {
public:
	AISystemDemoTest(EntityManager& entityManager);
	AISystem aiSystem;
	AIComponent newAIComponent;
	MapComponent newMap;
	GridPositionComponent newPos;
};


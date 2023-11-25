#pragma once
#include "ecs/entity/EntityManager.h"

class EnemyTurnCalculator {
public:
	EntityManager* entityManager;
	EnemyTurnCalculator(EntityManager* e);

}
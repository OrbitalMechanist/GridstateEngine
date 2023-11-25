#pragma once
#include "ecs/entity/EntityManager.h"

class EnemyTurnCalculator {
public:
	std::mutex mutex;
	std::condition_variable conditionVariable;
	bool ready;
	EntityManager* entityManager;
	EnemyTurnCalculator(EntityManager* e);
	~EnemyTurnCalculator();
	void executeEnemyTurns();
	void executeEnemyTurn(Entity* e);
};
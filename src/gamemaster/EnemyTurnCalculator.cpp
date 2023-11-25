#include "gamemaster/EnemyTurnCalculator.h"
#include <thread>

EnemyTurnCalculator::EnemyTurnCalculator(EntityManager* e) : entityManager(e) {}
EnemyTurnCalculator::~EnemyTurnCalculator() {}

void EnemyTurnCalculator::executeEnemyTurns()
{
	std::vector<Entity> entities = entityManager->getEntitiesWithComponent<TransformComponent>();
	std::thread *threads = new std::thread[entities.size()];
	for (int i = 0; i < entities.size(); i++) {
		threads[i] = std::thread(&EnemyTurnCalculator::executeEnemyTurn, this, &entities[i]);
		threads[i].join();
	}
	
}

void EnemyTurnCalculator::executeEnemyTurn(Entity* e)
{
	entityManager->getComponent<TransformComponent>(*e).pos.x += 1;
}

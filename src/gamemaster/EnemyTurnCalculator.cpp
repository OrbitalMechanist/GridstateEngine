#include "gamemaster/EnemyTurnCalculator.h"


EnemyTurnCalculator::EnemyTurnCalculator(EntityManager* e) : entityManager(e), ready(true) {}
EnemyTurnCalculator::~EnemyTurnCalculator() {}

void EnemyTurnCalculator::executeEnemyTurns()
{
	std::vector<Entity> entities = entityManager->getEntitiesWithComponent<AIComponent>();
	std::thread *threads = new std::thread[entities.size()];
	for (int i = 0; i < entities.size(); i++) {
		threads[i] = std::thread(&EnemyTurnCalculator::executeEnemyTurn, this, &entities[i]);
		threads[i].join();
	}
	delete[] threads;
}

void EnemyTurnCalculator::executeEnemyTurn(Entity* e)
{
	std::unique_lock<std::mutex> lock(mutex);
	while (!ready) {
		conditionVariable.wait(lock);
	}
	ready = false;
	//Replace with AI stuff here
	//entityManager->getComponent<TransformComponent>(*e).pos.x += 1;
	
	//
	ready = true;
	lock.unlock();
	conditionVariable.notify_one();
}

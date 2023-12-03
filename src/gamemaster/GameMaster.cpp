#include "gamemaster/GameMaster.h"

GameMaster::GameMaster(EntityManager* e) : currentTurn(playerTurn), entityManager(e), currentMode(select) {}

GameMaster::~GameMaster() {}

void GameMaster::endTurn() {
	if (currentTurn == playerTurn) {
		currentTurn = enemyTurn;
		std::vector<Entity> entitiesWithPlayer = entityManager->getEntitiesWithComponent<PlayerComponent>();
		for (auto entity : entitiesWithPlayer) {
			entityManager->getComponent<MoveComponent>(entity).moved = true;
		}
	}
	else if (currentTurn == enemyTurn) {
		currentTurn = playerTurn;
		std::vector<Entity> entitiesWithPlayer = entityManager->getEntitiesWithComponent<PlayerComponent>();
		for (auto entity : entitiesWithPlayer) {
			entityManager->getComponent<MoveComponent>(entity).moved = false;
		}
	}
	startTurn(currentTurn);
}

void GameMaster::startTurn(Turn t) {
	//ping whoever its their turn
}

void GameMaster::switchMode(Mode mode) {
	currentMode = mode;
}
#include "gamemaster/GameMaster.h"

GameMaster::GameMaster(EntityManager* e) : currentTurn(enemyTurn), entityManager(e) {}

GameMaster::~GameMaster() {}

void GameMaster::endTurn() {
	if (currentTurn == playerTurn) {
		currentTurn = enemyTurn;

	}
	else if (currentTurn == enemyTurn) {
		currentTurn = playerTurn;
	}
	startTurn(currentTurn);
}

void GameMaster::startTurn(Turn t) {
	//ping whoever its their turn
}
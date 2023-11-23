#include "gamemaster/GameMaster.h"

GameMaster::GameMaster(EntityManager* e) : currentTurn(playerTurn), entityManager(e) {}

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
	if (t == playerTurn) {

	}
	else if (t == enemyTurn) {

	}
}
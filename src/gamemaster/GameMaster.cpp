#include "gamemaster/GameMaster.h"

GameMaster::GameMaster(EnemyTurnCalculator* e) : currentTurn(playerTurn), calc(e) {}

GameMaster::~GameMaster() {}

void GameMaster::endTurn() {
	if (currentTurn == playerTurn) {
		currentTurn = enemyTurn;

	}
	else if (currentTurn == enemyTurn) {
		currentTurn = playerTurn;
	}
	startTurn();
}

void GameMaster::startTurn() {
	if (currentTurn == playerTurn) {
		// Give player control & refresh # of moves?
	}
	else if (currentTurn == enemyTurn) {
		// Revoke player control and execute enemy turns
		calc->executeEnemyTurns();
		//endTurn();
	}
}
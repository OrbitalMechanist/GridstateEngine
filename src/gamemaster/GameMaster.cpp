#include "gamemaster/GameMaster.h"

GameMaster::GameMaster(EntityManager e) : currentTurn(0), entities(e) {}

GameMaster::~GameMaster() {}

void GameMaster::endTurn() {
	//Keeps tracks of the total amount of turns based off each enemy and player character
	int lastTurn = 0;
	for (Entity e : entities.getEntitiesWithComponent<AIComponent>()) {
		lastTurn += 1;
	}
	if (currentTurn = lastTurn) {
		currentTurn = 0;
		//Ping player
	}
	else {
		currentTurn++;
		//Ping enemies
	}
}
#pragma once
#include "ecs/entity/EntityManager.h"

enum Turn {
	enemyTurn,
	playerTurn,
	nullTurn
};

class GameMaster {
public:
	Turn currentTurn;
	EntityManager entities;

	GameMaster(EntityManager e);

	~GameMaster();

	void endTurn();

	void startTurn(Turn t);
};
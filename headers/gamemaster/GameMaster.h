#pragma once
#include "gamemaster/EnemyTurnCalculator.h"

enum Turn {
	enemyTurn,
	playerTurn,
	nullTurn
};

class GameMaster {
public:
	Turn currentTurn;
	EnemyTurnCalculator* calc;

	GameMaster(EnemyTurnCalculator* e);

	~GameMaster();

	void endTurn();

	void startTurn();
};
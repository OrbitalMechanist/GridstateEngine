#pragma once
#include "ecs/entity/EntityManager.h"

enum Turn {
	enemyTurn,
	playerTurn,
	nullTurn
};
enum Mode {
	select,
	move,
	attack
};

class GameMaster {
public:
	Turn currentTurn;
	EntityManager* entityManager;
	Entity selected;
	Mode currentMode;
	bool botSelected;

	GameMaster(EntityManager* e);

	~GameMaster();

	void endTurn();

	void startTurn(Turn t);

	void switchMode(Mode mode);

	void selectUnit(int x, int y);

	bool moveSelected(int x, int y);

	bool attackSelected(int x, int y);
};
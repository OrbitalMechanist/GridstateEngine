#pragma once
#include "ecs/entity/EntityManager.h"

class GameMaster {
public:
	int currentTurn;
	EntityManager entities;

	GameMaster(EntityManager e);

	~GameMaster();

	void endTurn();
};
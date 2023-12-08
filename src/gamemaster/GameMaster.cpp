#include "gamemaster/GameMaster.h"

GameMaster::GameMaster(EntityManager* e, AudioManager* a) : currentTurn(playerTurn), entityManager(e), audioManager(a), currentMode(select), botSelected(false) {}

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

void GameMaster::selectUnit(int x, int y) {
	std::vector<Entity> entitiesWithPlayers = entityManager->getEntitiesWithComponent<PlayerComponent>();
	std::vector<Entity> entitiesWithAI = entityManager->getEntitiesWithComponent<AIComponent>();
	for (auto entity : entitiesWithPlayers) {
		if (entityManager->getComponent<TransformComponent>(entity).pos.x == x && entityManager->getComponent<TransformComponent>(entity).pos.y == y) {
			selected = entity;
			botSelected = false;
		}
	}
	for (auto entity : entitiesWithAI) {
		if (entityManager->getComponent<TransformComponent>(entity).pos.x == x && entityManager->getComponent<TransformComponent>(entity).pos.y == y) {
			selected = entity;
			botSelected = true;
		}
	}
}

bool GameMaster::moveSelected(int x, int y) {
	if (selected != NULL && !botSelected) {
		std::vector<Entity> entitiesWithPlayer = entityManager->getEntitiesWithComponent<PlayerComponent>();
		std::vector<Entity> entitiesWithAI = entityManager->getEntitiesWithComponent<AIComponent>();
		std::vector<Entity> entitiesWithObstacle = entityManager->getEntitiesWithComponent<ObstacleComponent>();
		auto moveComp = entityManager->getComponent<MoveComponent>(selected);
		auto transComp = entityManager->getComponent<TransformComponent>(selected);
		int moveAmount = moveComp.moveRange;
		bool canMove = false;
		bool noStack = true;
		if (!moveComp.moved) {
			if (0 <= x && x <= 10 && 0 <= y && y <= 10) {
				if (moveAmount - (abs(transComp.pos.x - x) + abs(transComp.pos.y - y)) >= 0) {
					canMove = true;
				}
				if (canMove) {
					for (auto entity : entitiesWithPlayer) {
						if (entityManager->getComponent<TransformComponent>(entity).pos.x == x && entityManager->getComponent<TransformComponent>(entity).pos.y == y && entity != selected) {
							noStack = false;
							break;
						}
					}
					for (auto entity : entitiesWithAI) {
						if (entityManager->getComponent<TransformComponent>(entity).pos.x == x && entityManager->getComponent<TransformComponent>(entity).pos.y == y && entity != selected) {
							noStack = false;
							break;
						}
					}
					for (auto entity : entitiesWithObstacle) {
						if (entityManager->getComponent<TransformComponent>(entity).pos.x == x && entityManager->getComponent<TransformComponent>(entity).pos.y == y && entity != selected) {
							noStack = false;
							break;
						}
					}
					if (noStack) {
						entityManager->getComponent<MoveComponent>(selected).moved = true;
						entityManager->getComponent<TransformComponent>(selected).pos = { x, y };
						selected = NULL;
						currentMode = select;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool GameMaster::attackSelected(int x, int y) {
	if (selected != NULL && !botSelected) {
		std::vector<Entity> entitiesWithPlayer = entityManager->getEntitiesWithComponent<PlayerComponent>();
		std::vector<Entity> entitiesWithAI = entityManager->getEntitiesWithComponent<AIComponent>();
		std::vector<Entity> entitiesWithObstacle = entityManager->getEntitiesWithComponent<ObstacleComponent>();
		auto unitAtk = entityManager->getComponent<AttackComponent>(selected);
		auto transComp = entityManager->getComponent<TransformComponent>(selected);
		int atkRange = unitAtk.range;
		bool canAttack = false;
		bool foundEnemy = false;
		Entity enemyUnit;
		if (!entityManager->getComponent<MoveComponent>(selected).moved) {
			if (atkRange - (abs(transComp.pos.x - x) + abs(transComp.pos.y - y)) >= 0) {
				canAttack = true;
			}
			if (canAttack) {
				for (auto entity : entitiesWithAI) {
					if (entityManager->getComponent<TransformComponent>(entity).pos.x == x && entityManager->getComponent<TransformComponent>(entity).pos.y == y && entity != selected) {
						foundEnemy = true;
						enemyUnit = entity;
						break;
					}
				}
				for (auto entity : entitiesWithObstacle) {
					if (entityManager->getComponent<TransformComponent>(entity).pos.x == x && entityManager->getComponent<TransformComponent>(entity).pos.y == y && entity != selected) {
						foundEnemy = true;
						enemyUnit = entity;
						break;
					}
				}
				if (foundEnemy) {
					entityManager->getComponent<MoveComponent>(selected).moved = true;
					auto unitDef = entityManager->getComponent<HealthComponent>(enemyUnit);
					if (unitDef.health > unitAtk.damage - unitDef.armor) {
						entityManager->getComponent<HealthComponent>(enemyUnit).health -= unitAtk.damage - unitDef.armor;
					}
					else {
						entityManager->destroyEntity(enemyUnit);
					}
					glm::vec3 enemyPos = { entityManager->getComponent<TransformComponent>(enemyUnit).pos.x, 0,entityManager->getComponent<TransformComponent>(enemyUnit).pos.y};
					entityManager->getComponent<AudioComponent>(enemyUnit).sourceA->SetPosition(enemyPos);
					entityManager->getComponent<AudioComponent>(enemyUnit).sourceA->Play(audioManager->getSoundEffect("spellHit"));
					entityManager->getComponent<AudioComponent>(enemyUnit).sourceB->SetPosition(enemyPos);
					entityManager->getComponent<AudioComponent>(enemyUnit).sourceB->Play(audioManager->getSoundEffect("injured"));
					selected = NULL;
					currentMode = select;
					return true;
				}
			}
		}
		return false;
	}
}
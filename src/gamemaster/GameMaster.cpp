#include "gamemaster/GameMaster.h"

GameMaster::GameMaster(EntityManager* e, AudioManager* a) : currentTurn(playerTurn), entityManager(e), audioManager(a), currentMode(select), botSelected(false) 
{
	uiAudio = new SoundSource(1.f, 1.f, { 0.0f,0.0f,0.0f }, { 0,0,0 }, false, false);
}

GameMaster::~GameMaster() {}

void GameMaster::endTurn() {
	if (currentTurn == playerTurn) {
		currentTurn = enemyTurn;
		std::vector<Entity> entitiesWithPlayer = entityManager->getEntitiesWithComponent<PlayerComponent>();
		for (auto entity : entitiesWithPlayer) {
			entityManager->getComponent<MoveComponent>(entity).moved = true;
		}
		uiAudio->Play(audioManager->getSoundEffect("buttonClick"));
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
	uiAudio->Play(audioManager->getSoundEffect("buttonClick"));
}

void GameMaster::selectUnit(int x, int y) {
	std::vector<Entity> entitiesWithPlayers = entityManager->getEntitiesWithComponent<PlayerComponent>();
	std::vector<Entity> entitiesWithAI = entityManager->getEntitiesWithComponent<AIComponent>();
	bool valid = false;
	for (auto entity : entitiesWithPlayers) {
		if (entityManager->getComponent<TransformComponent>(entity).pos.x == x && entityManager->getComponent<TransformComponent>(entity).pos.y == y) {
			selected = entity;
			botSelected = false;
			uiAudio->Play(audioManager->getSoundEffect("select"));
			valid = true;
		}
	}
	for (auto entity : entitiesWithAI) {
		if (entityManager->getComponent<TransformComponent>(entity).pos.x == x && entityManager->getComponent<TransformComponent>(entity).pos.y == y) {
			selected = entity;
			botSelected = true;
			uiAudio->Play(audioManager->getSoundEffect("select"));
			valid = true;
		}
	}
	if (!valid) {
		uiAudio->Play(audioManager->getSoundEffect("invalid"));
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

						glm::vec3 selectedPos = { entityManager->getComponent<TransformComponent>(selected).pos.x, entityManager->getComponent<TransformComponent>(selected).pos.y, 0 };
						entityManager->getComponent<AudioComponent>(selected).sourceA->SetPosition(selectedPos);
						entityManager->getComponent<AudioComponent>(selected).sourceA->Play(audioManager->getSoundEffect("move"));

						selected = NULL;
						currentMode = select;				
						return true;
					}
					else {
						uiAudio->Play(audioManager->getSoundEffect("invalid"));
					}
				}
				else {
					uiAudio->Play(audioManager->getSoundEffect("invalid"));
				}
			}
			else {
				uiAudio->Play(audioManager->getSoundEffect("invalid"));
			}
		}
		else {
			uiAudio->Play(audioManager->getSoundEffect("invalid"));
		}
	}
	else {
		uiAudio->Play(audioManager->getSoundEffect("invalid"));
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
		bool isAI = false;
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
						isAI = true;
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
					if (unitAtk.damage - unitDef.armor > 0) {
						if (unitDef.health > unitAtk.damage - unitDef.armor) {
							entityManager->getComponent<HealthComponent>(enemyUnit).health -= unitAtk.damage - unitDef.armor;
						}
						else {
							entityManager->destroyEntity(enemyUnit);
						}
					}
					glm::vec3 enemyPos = { entityManager->getComponent<TransformComponent>(enemyUnit).pos.x, entityManager->getComponent<TransformComponent>(enemyUnit).pos.y, 0};
					entityManager->getComponent<AudioComponent>(enemyUnit).sourceA->SetPosition(enemyPos);
					entityManager->getComponent<AudioComponent>(enemyUnit).sourceA->Play(audioManager->getSoundEffect("spellHit"));
					if (isAI) {
						entityManager->getComponent<AudioComponent>(enemyUnit).sourceB->SetPosition(enemyPos);
						entityManager->getComponent<AudioComponent>(enemyUnit).sourceB->Play(audioManager->getSoundEffect("injured"));
					}
					
					selected = NULL;
					currentMode = select;
					return true;
				}
			}
		}
		uiAudio->Play(audioManager->getSoundEffect("invalid"));
		return false;
	}
}
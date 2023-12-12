#include "gamemaster/GameMaster.h"

GameMaster::GameMaster(EntityManager* e, AudioManager* a) : currentTurn(playerTurn), entityManager(e), audioManager(a), currentMode(select), botSelected(false) 
{
	uiAudio = new SoundSource(1.f, 1.f, { 0.0f,0.0f,0.0f }, { 0,0,0 }, false, false);
}

GameMaster::~GameMaster() {}

//Ends the current active persons turn player or ai
void GameMaster::endTurn() {
	//If its player change turn to enemy turn
	if (currentTurn == playerTurn) {
		currentTurn = enemyTurn;
		//Allows all entities with player component to move once.
		std::vector<Entity> entitiesWithPlayer = entityManager->getEntitiesWithComponent<PlayerComponent>();
		for (auto entity : entitiesWithPlayer) {
			entityManager->getComponent<MoveComponent>(entity).moved = true;
		}
		uiAudio->Play(audioManager->getSoundEffect("buttonClick"));
	}
	//If its enemy turn to player turn
	else if (currentTurn == enemyTurn) {
		currentTurn = playerTurn;
		std::vector<Entity> entitiesWithPlayer = entityManager->getEntitiesWithComponent<PlayerComponent>();
		//Doesn't allow any entities with player component to move anymore.
		for (auto entity : entitiesWithPlayer) {
			entityManager->getComponent<MoveComponent>(entity).moved = false;
		}
	}
}

//Switches mode to given mode
void GameMaster::switchMode(Mode mode) {
	currentMode = mode;
	//Plays button click audio
	uiAudio->Play(audioManager->getSoundEffect("buttonClick"));
}

//Selects unit at the given grid space of x and y
void GameMaster::selectUnit(int x, int y) {
	std::vector<Entity> entitiesWithPlayers = entityManager->getEntitiesWithComponent<PlayerComponent>();
	std::vector<Entity> entitiesWithAI = entityManager->getEntitiesWithComponent<AIComponent>();
	//Boolean to check if the selection is valid
	bool valid = false;
	// If there is an entity with player component on that grid space select
	for (auto entity : entitiesWithPlayers) {
		if (entityManager->getComponent<TransformComponent>(entity).pos.x == x && entityManager->getComponent<TransformComponent>(entity).pos.y == y) {
			//Sets selected to be the entity on this space
			selected = entity;
			//Boolean to check if ai is selected
			botSelected = false;
			uiAudio->Play(audioManager->getSoundEffect("select"));
			valid = true;
		}
	}
	//If there is an entity with ai component on that grid space select.
	for (auto entity : entitiesWithAI) {
		if (entityManager->getComponent<TransformComponent>(entity).pos.x == x && entityManager->getComponent<TransformComponent>(entity).pos.y == y) {
			//Sets selected to be the entity on this space
			selected = entity;
			//Boolean to check if ai is selected
			botSelected = true;
			uiAudio->Play(audioManager->getSoundEffect("select"));
			valid = true;
		}
	}
	//If not valid play invalid sound
	if (!valid) {
		uiAudio->Play(audioManager->getSoundEffect("invalid"));
	}
}

bool GameMaster::moveSelected(int x, int y) {
	//If entity is selected and its not bot allow move
	if (selected != NULL && !botSelected) {
		std::vector<Entity> entitiesWithPlayer = entityManager->getEntitiesWithComponent<PlayerComponent>();
		std::vector<Entity> entitiesWithAI = entityManager->getEntitiesWithComponent<AIComponent>();
		std::vector<Entity> entitiesWithObstacle = entityManager->getEntitiesWithComponent<ObstacleComponent>();
		//Gets move and transform component of the selected entity.
		auto moveComp = entityManager->getComponent<MoveComponent>(selected);
		auto transComp = entityManager->getComponent<TransformComponent>(selected);
		int moveAmount = moveComp.moveRange;
		//Boolean to check if the entity canMove and theres would be no 2 entities stacked on the same grid
		bool canMove = false;
		bool noStack = true;
		if (!moveComp.moved) {
			//If its within bounds of the 11x11 grid allow movement
			if (0 <= x && x <= 10 && 0 <= y && y <= 10) {
				//If the grid space is within the move amount range allow movement
				if (moveAmount - (abs(transComp.pos.x - x) + abs(transComp.pos.y - y)) >= 0) {
					canMove = true;
				}
				//If can move check if theres an another unit on the space.
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
					//If not stacked move the entity
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
	//If entity is selected and its not bot allow move
	if (selected != NULL && !botSelected) {
		std::vector<Entity> entitiesWithPlayer = entityManager->getEntitiesWithComponent<PlayerComponent>();
		std::vector<Entity> entitiesWithAI = entityManager->getEntitiesWithComponent<AIComponent>();
		std::vector<Entity> entitiesWithObstacle = entityManager->getEntitiesWithComponent<ObstacleComponent>();
		//Variables containing the units attack and position
		auto unitAtk = entityManager->getComponent<AttackComponent>(selected);
		auto transComp = entityManager->getComponent<TransformComponent>(selected);
		int atkRange = unitAtk.range;
		//If the unit can attack and if they found an enemy on the space
		bool canAttack = false;
		bool foundEnemy = false;
		bool isAI = false;
		Entity enemyUnit;
		//If the unit hasn't done an action already allow attack
		if (!entityManager->getComponent<MoveComponent>(selected).moved) {
			//If the entity its attacking is within attack range.
			if (atkRange - (abs(transComp.pos.x - x) + abs(transComp.pos.y - y)) >= 0) {
				canAttack = true;
			}
			//If can attack check whether its an AI or obstacle
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
				//If enemy is found allow the attack
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
					//Set audio source to enemy position and play sound
					entityManager->getComponent<AudioComponent>(enemyUnit).sourceA->SetPosition(enemyPos);
					entityManager->getComponent<AudioComponent>(enemyUnit).sourceA->Play(audioManager->getSoundEffect("spellHit"));
					//If its an ai attack play injured sound
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
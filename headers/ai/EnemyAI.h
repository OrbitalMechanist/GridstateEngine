//
// Created by Hushe on 11/26/2023.
// Edited by Mike
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "ecs/entity/EntityManager.h"
#include "audio/AudioManager.h"
#include "ecs/components/GridPositionComponent.h"
#include "ecs/components/AttackComponent.h"
#include "ecs/components/HealthComponent.h"
#include "ecs/components/PlayerComponent.h"

#ifndef UNTITLED_ENEMYAI_H
#define UNTITLED_ENEMYAI_H


class EnemyAI {
	EntityManager& manager;
	AudioManager& audio;
public:
	EnemyAI(EntityManager& manager, AudioManager& audio) : manager(manager), audio(audio) {};
	Entity GetClosestPlayer(Entity attacker); // attack the closest player
	double calculateDistance(std::pair<int, int> pos1, std::pair<int, int> pos2);
	void enemyPerform(Entity attacker, Entity target);
private:
	
	
	bool performAttack(Entity attacker, Entity target);
	float calculateHitChance(Entity attacker, Entity target);
	
};


#endif //UNTITLED_ENEMYAI_H

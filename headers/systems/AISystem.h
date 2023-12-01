#pragma once
#include <iostream>
#include "ecs/entity/EntityManager.h"
#include "ai/Pathfinding.h"
#include "ai/EnemyAI.h"
#include "Systems/MessagingSystem/MessageSystem.h"
#include "gamemaster/EnemyTurnCalculator.h"
#include "gamemaster/GameMaster.h"

#ifndef AISYSTEM_H
#define AISYSTEM_H


class AISystem {
    EntityManager& manager;
    MessageBus& bus;
    GameMaster* gm;
public:
    // Register type
    AISystem(EntityManager& manager, MessageBus& bus, GameMaster* gm) : manager(manager), bus(bus), gm(gm) {};
    // Update function to be called every game loop iteration.
    void update(); 
    void spawnEnemy();
    void startEnemyTurn();

private:
    // states handler functions
    void handleIdleState(Entity et);
    void handlePathfindingState(Entity et);
    void handleAttackState(Entity et);
    void handleTakeCoverState(Entity et);
    int entityID = 0; 
    int hasAttackCount = 0; // use this to check if all enemy has finished 
};




#endif

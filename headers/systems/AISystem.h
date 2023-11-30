#pragma once
#include <iostream>
#include "ecs/entity/EntityManager.h"
#include "ai/Pathfinding.h"
#include "ai/EnemyAI.h"
#include "Systems/MessagingSystem/MessageSystem.h"

#ifndef AISYSTEM_H
#define AISYSTEM_H


class AISystem {
    EntityManager& manager;
    MessageBus& bus;
  
public:
    // Register type
    AISystem(EntityManager& manager, MessageBus& bus) : manager(manager), bus(bus)  {};
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
    int entityID;
    bool isEnemyTurn = true;
};




#endif

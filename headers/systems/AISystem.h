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
    std::array<std::array<int, 11>, 11> map = {}; // gameMap
public:
    // Register type
    AISystem(EntityManager& manager, MessageBus& bus, GameMaster* gm, const std::array<std::array<int, 11>, 11>& map) : manager(manager), bus(bus), gm(gm), map(map) {};
    // Update function to be called every game loop iteration.
    void update(); 
    void spawnEnemy();
    void startEnemyTurn();
    //void updateMap(int x, int y, int flag); // flag is 0 - 1 -2 : obstacle,unoccupied,player/enemy

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

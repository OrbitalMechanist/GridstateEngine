#pragma once
#include <iostream>
#include "ecs/entity/EntityManager.h"
#include "ai/Pathfinding.h"
#include "ai/EnemyAI.h"
#include "Systems/MessagingSystem/MessageSystem.h"
#include "gamemaster/GameMaster.h"
#include <mutex>
#include <NsGui/TextBlock.h>
#include <thread>
#include <vector>
#include <cassert>
#include <iostream>
#include <string>
#ifndef AISYSTEM_H
#define AISYSTEM_H

class AISystem {
    EntityManager& manager;
    MessageBus& bus;
    GameMaster& gm;
   
public:
    AISystem(EntityManager& manager, MessageBus& bus, GameMaster& gm) : manager(manager), bus(bus), gm(gm) {};
    // Update function to be called every game loop iteration.
    void update();
    void spawnEnemy(TransformComponent trans, StaticMeshComponent stat);

private:
    // states handler functions
   // void handleIdleState(Entity et);
    void handlePathfindingState(Entity et);
    void handleAttackState(Entity et);
   // void handleTakeCoverState(Entity et);
    int entityID = 0;
    int hasAttackCount = 0; // use this to check if all enemy has finished

    const int MAX_Y = 11; // size of map
    int map[11][11] = {}; // gameMap
    void updateMap();
    void processAIEntity(Entity entity);
    void updateAI(std::vector<Entity>& entities);
    bool isUpdate = false;
    std::mutex mtx;
    std::mutex mtx2;
    std::mutex mapMtx;
};




#endif

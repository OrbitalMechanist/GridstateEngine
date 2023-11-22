#pragma once
#include <iostream>

#ifndef AISYSTEM_H
#define AISYSTEM_H

#include "../entity/EntityManager.h"
#include "../ai/Pathfinding.h"
class AISystem {
public:
    // Update function to be called every game loop iteration.
    void update(EntityManager& manager, float dt); // dt - delta time

private:
    // states handler functions
    void handleIdleState(Entity entity, EntityManager& manager, float dt);
    void handlePathfindingState(Entity entity, EntityManager& manager, float dt);
    void handleAttackState(Entity entity, EntityManager& manager, float dt);
    void handleTakeCoverState(Entity entity, EntityManager& manager, float dt);
   
};




#endif

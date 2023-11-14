#pragma once

#include "../entity/EntityManager.h" // placeholder - need a EntityManager and Entiy to handle different states


class AISystem {
public:
   void update(EntityManager& entityManager, float dt); // dt is the delta time
private:
   void handleIdleState(Entity& entity, float dt);
   void handlePathfindingState(Entity& entity, float dt);
   void handleAttackState(Entity& entity, float dt);
   void handleTakeCoverState(Entity& entity, float dt);
};
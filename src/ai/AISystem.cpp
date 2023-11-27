#include "./ai/AISystem.h"

void AISystem::update(EntityManager& manager, float dt) {
    std::vector<Entity> entitiesWithAI = manager.getEntitiesWithComponent<AIComponent>();
    //std::cout << "Test " << "" << std::endl;
    for (auto entity : entitiesWithAI) {
        auto& aicomponent = manager.getComponent<AIComponent>(entity);
        // states
        switch (aicomponent.state) {
        case AIState::Idle:

            // idle
            handleIdleState(entity, manager, dt);
            break;
        case AIState::Pathfinding:
            // pathfinding
            handlePathfindingState(entity, manager, dt);
            break;
        case AIState::Attack:
            // attack
            handleAttackState(entity, manager, dt);
            break;
        case AIState::TakeCover:
            // take cover
            handleTakeCoverState(entity, manager, dt);
            break;
        }
    }
}

void AISystem::handleIdleState(Entity& entity, EntityManager& manager, float dt) {
    // Idle state
    std::cout << "Idle State" << std::endl;
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
    aiComponent.state = AIState::Attack;
    std::cout << "Switching to attack state" << std::endl;
}

void AISystem::handlePathfindingState(Entity& entity, EntityManager& manager, float dt) {
    // Pathfinding state
    std::cout << "Pathfinding State" << std::endl;
}

void AISystem::handleAttackState(Entity& entity, EntityManager& manager, float dt) {
    // Attack state
    std::cout << "Attack State" << std::endl;
}

void AISystem::handleTakeCoverState(Entity& entity, EntityManager& manager, float dt) {
    // Take cover state
    std::cout << "Take Cover State" << std::endl;
}

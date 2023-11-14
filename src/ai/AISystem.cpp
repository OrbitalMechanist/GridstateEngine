#include "./ai/AISystem.h"

void AISystem::update(EntityManager& entityManager, float dt) {
    // Get all entities with a StateComponent
    auto view = entityManager.view<StateComponent>();

    for (auto entity : view) {
        auto& state = entityManager.getComponent<StateComponent>(entity);

        // Determine behavior based on state
        switch (state.currentState) {
        case AIState::IDLE:
            handleIdleState(entity, dt);
            break;
        case AIState::PATHFINDING:
            handlePathfindingState(entity, dt);
            break;
        case AIState::ATTACK:
            handleAttackState(entity, dt);
            break;
        case AIState::TAKE_COVER:
            handleTakeCoverState(entity, dt);
            break;
        }
    }
}

void AISystem::handleIdleState(Entity& entity, float dt) {
    // Idle state
    std::cout << "Idle State" << std::endl;
}

void AISystem::handlePathfindingState(Entity& entity, float dt) {
    // Pathfinding state
    std::cout << "Pathfinding State" << std::endl;
}

void AISystem::handleAttackState(Entity& entity, float dt) {
    // Attack state
    std::cout << "Attack State" << std::endl;
}

void AISystem::handleTakeCoverState(Entity& entity, float dt) {
    // Take cover state
    std::cout << "Take Cover State" << std::endl;
}

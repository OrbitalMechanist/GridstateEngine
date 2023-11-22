#include "../headers/Systems/AISystem.h"


void AISystem::update(EntityManager& manager, float dt) {
    std::vector<Entity> entitiesWithAI = manager.getEntitiesWithComponent<AIComponent>();
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

// idle state handler
void AISystem::handleIdleState(Entity entity, EntityManager& manager, float dt){
    std::cout << "Idle state" << std::endl;
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
    aiComponent.state = AIState::Pathfinding;
}

// Pathfinding state handler
void AISystem::handlePathfindingState(Entity entity, EntityManager& manager, float dt){
    std::cout << "Pathfinding state" << std::endl;
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
    MapComponent& mapComponent = manager.getComponent<MapComponent>(entity);
    GridPositionComponent& posComponent = manager.getComponent<GridPositionComponent>(entity);
    std::pair aiPosition = std::make_pair(posComponent.gridX, posComponent.gridY);
    std::pair playerPosition = std::make_pair(1, 1);
    // pathfinding
    Pathfinding path;
    path.aStarSearch(mapComponent.map, aiPosition, playerPosition);
    path.printDirVec();
    //aiComponent.state = AIState::Attack;
}

// Attack state handler
void AISystem::handleAttackState(Entity entity, EntityManager& manager, float dt){
    std::cout << "Attack state" << std::endl;
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
   
    aiComponent.state = AIState::TakeCover;
}

// Takecover state handler
void AISystem::handleTakeCoverState(Entity entity, EntityManager& manager, float dt){
    std::cout << "Takecover state" << std::endl;
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
    aiComponent.state = AIState::Idle;
}
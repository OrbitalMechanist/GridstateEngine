#include "../headers/Systems/AISystem.h"


void AISystem::update() {
    std::vector<Entity> entitiesWithAI = manager.getEntitiesWithComponent<AIComponent>();
    for (auto entity : entitiesWithAI) {
        auto& aicomponent = manager.getComponent<AIComponent>(entity);
        // states
        switch (aicomponent.state) {
            case AIState::Idle:      
                // idle
                handleIdleState(entity);
                break;
            case AIState::Pathfinding:
                // pathfinding
                handlePathfindingState(entity);
                break;
            case AIState::Attack:
                // attack
                handleAttackState(entity);
                break;
            case AIState::TakeCover:
                // take cover
                handleTakeCoverState(entity);
                break;
        }   
    }
    if (gm->currentTurn == enemyTurn && hasAttackCount == entitiesWithAI.size()) {
        hasAttackCount = 0; // reset for next turn
        gm->endTurn();
        std::cout << "Enemy turn end" << std::endl;
    }
   

}

// idle state handler
void AISystem::handleIdleState(Entity entity){
    // swtich to pathfinding when enemy's turn started
    if (gm->currentTurn == enemyTurn) {
        std::cout << "Idle state" << std::endl;
        AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
        aiComponent.state = AIState::Pathfinding;
    }
}

// Pathfinding state handler
void AISystem::handlePathfindingState(Entity entity){
    std::cout << "Pathfinding state" << std::endl;
    int map[7][7] = {
    {1,1,1,1,1,1,1 },
    {1,1,1,1,1,1,1 },
    {1,1,1,1,1,1,1 },
    {1,1,1,1,1,2,1 },
    {1,1,1,1,1,1,1 },
    {1,1,1,1,1,1,1 },
    {1,1,1,1,1,1,1 },
    };
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
    EnemyAI aiPath(manager);
    Entity player = aiPath.GetClosestPlayer(entity);
    std::pair aiPosition = std::make_pair(manager.getComponent<TransformComponent>(entity).pos.x, manager.getComponent<TransformComponent>(entity).pos.y);
    std::pair playerPosition = std::make_pair(manager.getComponent<TransformComponent>(player).pos.x, manager.getComponent<TransformComponent>(player).pos.y);
    
    //// pathfinding
    Pathfinding path;
    path.aStarSearch(map, aiPosition, playerPosition);
    path.printDirVec();
    int walkRange = manager.getComponent<MoveComponent>(entity).moveRange;
  
    //std::cout << "new position = " << path.getNewPosition(walkRange).first << " : " << path.getNewPosition(walkRange).second << std::endl;
    if (path.getNewPosition(walkRange).first != NULL && path.getNewPosition(walkRange).second != NULL) {
        std::cout << "new position = " << path.GetDirMap().size() << std::endl;
        manager.getComponent<TransformComponent>(entity).pos.x = path.getNewPosition(walkRange).first;
        manager.getComponent<TransformComponent>(entity).pos.y = path.getNewPosition(walkRange).second;
    }
   


    // change state
    aiComponent.state = AIState::Attack;
}

// Attack state handler
void AISystem::handleAttackState(Entity entity){
    std::cout << "Attack state" << std::endl;
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);

    // find attack target
    EnemyAI aiAttack(manager);
    Entity target = aiAttack.GetClosestPlayer(entity);
    if (target == NULL) {
        std::cout << "NO player exited " << std::endl;
    }
    else{
        aiAttack.enemyPerform(entity, target);
        std::cout << "Current Health: " << manager.getComponent<HealthComponent>(target).health << std::endl;
    }

    hasAttackCount++;
    aiComponent.state = AIState::Idle;
}

// Takecover state handler - not using in this game
void AISystem::handleTakeCoverState(Entity entity){
    std::cout << "Takecover state" << std::endl;
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
   
    aiComponent.state = AIState::Idle;
}

// Spawn Enemy
void AISystem::spawnEnemy() {
    
    // create new aiEntity
    Entity aiEntity = manager.createEntity();
    

    // register
    manager.registerComponentType<AIComponent>();
    manager.registerComponentType<HealthComponent>();
    manager.registerComponentType<AttackComponent>();
    manager.registerComponentType<MoveComponent>();


 
    // increment entityID
    ++entityID;

    //// create component
    AIComponent ai(AIState::Idle);
    HealthComponent hp(bus,entityID, 100, 2); // assume health starts at 100 , armor 2
    AttackComponent att(1,2,1); // damage range and attackModifier = 1
    MoveComponent movement(2,false);


    //// add component
    
    manager.addComponent<AIComponent>(aiEntity, ai);
    manager.addComponent<HealthComponent>(aiEntity, hp);
    manager.addComponent<AttackComponent>(aiEntity, att);
    manager.addComponent<MoveComponent>(aiEntity, movement);



   

}

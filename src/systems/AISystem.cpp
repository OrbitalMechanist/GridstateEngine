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
    
    
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
    EnemyAI aiPath(manager);
    Entity player = aiPath.GetClosestPlayer(entity);

    // Get Position
    int aiPosX = manager.getComponent<TransformComponent>(entity).pos.x;
    int aiPosY = manager.getComponent<TransformComponent>(entity).pos.y;
    int playerPosX = manager.getComponent<TransformComponent>(player).pos.x;
    int playerPosY = manager.getComponent<TransformComponent>(player).pos.y;
   
    int gridAiPosX = aiPosX + 5;
    int gridAiPosY = 5 - aiPosY;

    int gridPlayerPosX = playerPosX + 5;
    int gridPlayerPosY = 5 - playerPosY;
   
  
    std::pair aiPosition = std::make_pair(gridAiPosX, gridAiPosY);
    std::pair playerPosition = std::make_pair(gridPlayerPosX, gridPlayerPosY);
    
    //// pathfinding
    Pathfinding path;
    int rawMap[11][11]; // saving time - if have time, chaneg this
    for (int i = 0; i < 11; ++i) {
        std::copy(map[i].begin(), map[i].end(), rawMap[i]);
    }
   // std::cout << "Before ai position = " << aiPosition.second << " : " << aiPosition.first << std::endl;
    //std::cout << "Before pla position = " << playerPosition.second << " : " << playerPosition.first << std::endl;
   
    path.aStarSearch(rawMap, aiPosition, playerPosition);
    //path.printDirVec();
    int walkRange = manager.getComponent<MoveComponent>(entity).moveRange;
    //std::cout << "Walk Range: " << walkRange << std::endl;
    //std::cout << "new position = " << path.getNewPosition(walkRange).second << " : " << path.getNewPosition(walkRange).first << std::endl;
    if (path.getNewPosition(walkRange).first != NULL && path.getNewPosition(walkRange).second != NULL) {
        //std::cout << "Grid position = " << gridAiPosY << " " << gridAiPosX << std::endl;
        map[gridAiPosY][gridAiPosX] = 1;
        int newX = path.getNewPosition(walkRange).first - 5;
        int newY = 5 - path.getNewPosition(walkRange).second ;

       // std::cout << "x " << newX << " : " << newY << std::endl;

        manager.getComponent<TransformComponent>(entity).pos.x = newX;
        manager.getComponent<TransformComponent>(entity).pos.y = newY;
        

        //map[path.getNewPosition(walkRange).second][path.getNewPosition(walkRange).first] = 2; // not sure why it has bug - fix this later
    }
   

    // change state
    aiComponent.state = AIState::Attack;
}
/*for (int i = 0; i <= 10; ++i) {
            for (int j = 0; j <= 10; ++j) {

                std::cout << map[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;*/

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
    

   


 
    // increment entityID
    ++entityID;

    //// create component
    AIComponent ai(AIState::Idle);
    HealthComponent hp(bus,entityID, 100, 2); // assume health starts at 100 , armor 2
    AttackComponent att(1,2,1); // damage range and attackModifier = 1
    MoveComponent movement(1,false);


    //// add component
    
    manager.addComponent<AIComponent>(aiEntity, ai);
    manager.addComponent<HealthComponent>(aiEntity, hp);
    manager.addComponent<AttackComponent>(aiEntity, att);
    manager.addComponent<MoveComponent>(aiEntity, movement);

}



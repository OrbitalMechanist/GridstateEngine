#include "../headers/Systems/AISystem.h"
#include <thread>

void AISystem::update(Noesis::TextBlock *turnText) {
    std::vector<Entity> entitiesWithAI = manager.getEntitiesWithComponent<AIComponent>();
    std::thread* threads = new std::thread[entitiesWithAI.size()];
    int count = 0;

    for (auto entity : entitiesWithAI) {
       
       
        auto& aicomponent = manager.getComponent<AIComponent>(entity);
        // states
        switch (aicomponent.state) {
            case AIState::Idle:
                // idle
                threads[count] = std::thread(&AISystem::handleIdleState, this, entity);
                threads[count].join();
                //handleIdleState(entity);
                break;
            case AIState::Pathfinding:
                // pathfinding
                threads[count] = std::thread(&AISystem::handlePathfindingState, this, entity);
                threads[count].join();
               // handlePathfindingState(entity);
                break;
            case AIState::Attack:
                // attack
                threads[count] = std::thread(&AISystem::handleAttackState, this, entity);
                threads[count].join();
               // handleAttackState(entity);
                break;
            case AIState::TakeCover:
                // take cover
                handleTakeCoverState(entity);
                break;
        }
        count++;
    }
    if (gm.currentTurn == enemyTurn && hasAttackCount == entitiesWithAI.size()) {
        hasAttackCount = 0; // reset for next turn
        turnText->SetText("Player Turn");
        gm.endTurn();
        std::cout << "Enemy turn end" << std::endl;
    }


}

// idle state handler
void AISystem::handleIdleState(Entity entity) {
    // swtich to pathfinding when enemy's turn started
    if (gm.currentTurn == enemyTurn) {
        std::cout << "Idle state" << std::endl;
        AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);

        

        // Switch to pathfinding State
        aiComponent.state = AIState::Pathfinding;
    }
}

// Pathfinding state handler
void AISystem::handlePathfindingState(Entity entity) {
    std::cout << "Pathfinding state" << std::endl;
    std::unique_lock<std::mutex> lock(mutex);
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
    if (manager.getEntitiesWithComponent<PlayerComponent>().size() > 0) {
       
        updateMap();

        // Find Closest player
        EnemyAI aiPath(manager);
        Entity player = aiPath.GetClosestPlayer(entity);

        // Get Position
        int aiPosX = manager.getComponent<TransformComponent>(entity).pos.x;
        int aiPosY = MAX_Y - manager.getComponent<TransformComponent>(entity).pos.y;
        int playerPosX = manager.getComponent<TransformComponent>(player).pos.x;
        int playerPosY = MAX_Y - manager.getComponent<TransformComponent>(player).pos.y;
        std::pair aiPosition = std::make_pair(aiPosX, aiPosY);
        std::pair playerPosition = std::make_pair(playerPosX, playerPosY);

        /*std::cout << "AIPOS: " << manager.getComponent<TransformComponent>(entity).pos.x << " : " << manager.getComponent<TransformComponent>(entity).pos.y << std::endl;
         std::cout << "player: " << manager.getComponent<TransformComponent>(player).pos.x << " : " << manager.getComponent<TransformComponent>(player).pos.y << std::endl;*/

         // pathfinding
        Pathfinding path;
        map[aiPosX][aiPosY] = 1;
        map[playerPosX][playerPosY] = 1;
        path.aStarSearch(map, aiPosition, playerPosition);
        map[aiPosX][aiPosY] = 0;
        map[playerPosX][playerPosY] = 2;
        path.printDirVec();

        // Move
        int walkRange = manager.getComponent<MoveComponent>(entity).moveRange;
        if (path.getNewPosition(walkRange).first != NULL && path.getNewPosition(walkRange).second != NULL) {

            int newX = path.getNewPosition(walkRange).first;
            int newY = MAX_Y - path.getNewPosition(walkRange).second;

            manager.getComponent<TransformComponent>(entity).pos.x = newX;
            manager.getComponent<TransformComponent>(entity).pos.y = newY;


        }
   
    }
    // Switch to Attack State
    aiComponent.state = AIState::Attack;
}

// Attack state handler
void AISystem::handleAttackState(Entity entity){
   // std::cout << "Attack state" << std::endl;
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
    if (manager.getEntitiesWithComponent<PlayerComponent>().size() > 0) {
        // find attack target
        EnemyAI aiAttack(manager);
        Entity target = aiAttack.GetClosestPlayer(entity);
        if (target == NULL) {
            std::cout << "NO player exited " << std::endl;
        }
        else {
            aiAttack.enemyPerform(entity, target);
            std::cout << "Current Health: " << manager.getComponent<HealthComponent>(target).health << std::endl;
        }
    }
    
    hasAttackCount++;
    aiComponent.state = AIState::Idle;
}

// Takecover state handler 
void AISystem::handleTakeCoverState(Entity entity){
    std::cout << "Takecover state" << std::endl;
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
    aiComponent.state = AIState::Idle;
}

// Spawn Enemy
void AISystem::spawnEnemy(TransformComponent trans, StaticMeshComponent stat) {

    // create new aiEntity
    Entity aiEntity = manager.createEntity();

    // increment entityID
    ++entityID;

    //// create component
    AIComponent ai(AIState::Idle);
    HealthComponent hp(entityID, 5, 2); // assume health starts at 5 , armor 2
    AttackComponent att(1, 2, 1); // damage range and attackModifier = 1
    MoveComponent movement(1, false);


    //// add component

    manager.addComponent<AIComponent>(aiEntity, ai);
    manager.addComponent<HealthComponent>(aiEntity, hp);
    manager.addComponent<AttackComponent>(aiEntity, att);
    manager.addComponent<MoveComponent>(aiEntity, movement);
    manager.addComponent<TransformComponent>(aiEntity, trans);
    manager.addComponent<StaticMeshComponent>(aiEntity, stat);
}


// not a efficient way - if have time, improve this
void AISystem::updateMap() {
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_Y; ++j) {
            map[i][j] = 1;
            //std::cout << map[i][j] << " ";
        }
        //std::cout << std::endl;
    }

    // slot taken by Enemy
    for (Entity aiEntity : manager.getEntitiesWithComponent<AIComponent>()) {
        
        int x = manager.getComponent<TransformComponent>(aiEntity).pos.x;
        int y = manager.getComponent<TransformComponent>(aiEntity).pos.y;
        int invertedY = MAX_Y - y;  
        //std::cout << x << " : " << y << std::endl;
        map[invertedY][x] = 0;
    }

    // slot taken by player
    for (Entity playerEntity : manager.getEntitiesWithComponent<PlayerComponent>()) {
        int x = manager.getComponent<TransformComponent>(playerEntity).pos.x;
        int y = manager.getComponent<TransformComponent>(playerEntity).pos.y;
        int invertedY = MAX_Y - y;
        map[invertedY][x] = 2;
    }

    /*std::cout << std::endl;
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_Y; ++j) {
           
            std::cout << map[i][j] << " ";
        }
        std::cout << std::endl;
        
    }*/
}

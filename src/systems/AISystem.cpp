#include "../headers/Systems/AISystem.h"


void AISystem::update() {
    std::vector<Entity> entitiesWithAI = manager.getEntitiesWithComponent<AIComponent>();
    if (gm.currentTurn == enemyTurn) {
        std::vector<Entity> entitiesWithAI = manager.getEntitiesWithComponent<AIComponent>();
        if(!isUpdate){
            isUpdate = true;
            updateAI(entitiesWithAI);
        }    
    }
    if (gm.currentTurn == enemyTurn && hasAttackCount == entitiesWithAI.size()) {
        hasAttackCount = 0; // reset for next turn
        isUpdate = false;
        gm.endTurn();
    }
}

// Pathfinding state handler
void AISystem::handlePathfindingState(Entity entity) {
    std::lock_guard<std::mutex> lock(mtx2);
    if (manager.getEntitiesWithComponent<PlayerComponent>().size() > 0) {
       
        updateMap();

        // Find Closest player
        EnemyAI aiPath(manager, *gm.audioManager);
        Entity player = aiPath.GetClosestPlayer(entity);

        // Get Position
        int aiPosX = manager.getComponent<TransformComponent>(entity).pos.x;
        int aiPosY = MAX_Y - manager.getComponent<TransformComponent>(entity).pos.y;
        int playerPosX = manager.getComponent<TransformComponent>(player).pos.x;
        int playerPosY = MAX_Y - manager.getComponent<TransformComponent>(player).pos.y;
        std::pair aiPosition = std::make_pair(aiPosX, aiPosY);
        std::pair playerPosition = std::make_pair(playerPosX, playerPosY);

         // pathfinding
        Pathfinding path;
        map[aiPosX][aiPosY] = 1;
        map[playerPosX][playerPosY] = 1; 
        path.aStarSearch(map, aiPosition, playerPosition);
        //path.printDirVec();
        

        // Move
        int walkRange = manager.getComponent<MoveComponent>(entity).moveRange;
       
        
        std::pair<int, int> newPos = path.getNewPosition(walkRange);
        
        int newX = newPos.first;
        int newY = newPos.second;

        if (newX != -1 &&  newY != -1) {
            newY = MAX_Y - newY;
            manager.getComponent<TransformComponent>(entity).pos.x = newX;
            manager.getComponent<TransformComponent>(entity).pos.y = newY;
            updateMap();
            if (aiPosX != newX && aiPosY != newY) {
                glm::vec3 entityPos = { manager.getComponent<TransformComponent>(entity).pos.x, manager.getComponent<TransformComponent>(entity).pos.y, 0 };
                manager.getComponent<AudioComponent>(entity).sourceB->SetPosition(entityPos);
                manager.getComponent<AudioComponent>(entity).sourceB->Play(gm.audioManager->getSoundEffect("move"));
            }
        }
    }
    
}

// Attack state handler
void AISystem::handleAttackState(Entity entity){
    if (manager.getEntitiesWithComponent<PlayerComponent>().size() > 0) {
        // find attack target
        EnemyAI aiAttack(manager, *gm.audioManager);
        Entity target = aiAttack.GetClosestPlayer(entity);
        if (target == NULL) {
            std::cout << "NO player exited " << std::endl;
        }
        else {
            aiAttack.enemyPerform(entity, target);  
        }
    }
    hasAttackCount++;
  
}


// Spawn Enemy, this is here for testing and should be replaced with a general UnitFactory later
void AISystem::spawnEnemy(TransformComponent trans, StaticMeshComponent stat) {

    // create new aiEntity
    Entity aiEntity = manager.createEntity();

    // increment entityID
    ++entityID;

    //// create component
    AIComponent ai(AIState::Idle);
    HealthComponent hp(entityID, 5, 2, 5); // assume health starts at 5 , armor 2, Max health 5
    AttackComponent att(3, 2, 1); // damage range and attackModifier = 1
    MoveComponent movement(1, false);
    AudioComponent audio;
    NameComponent name("Enemy Wizard");


    //// add component
    manager.addComponent<AIComponent>(aiEntity, ai);
    manager.addComponent<HealthComponent>(aiEntity, hp);
    manager.addComponent<AttackComponent>(aiEntity, att);
    manager.addComponent<MoveComponent>(aiEntity, movement);
    manager.addComponent<TransformComponent>(aiEntity, trans);
    manager.addComponent<StaticMeshComponent>(aiEntity, stat);
    manager.addComponent<AudioComponent>(aiEntity, audio);
    manager.addComponent<NameComponent>(aiEntity, name);
}


// not a efficient way - if have time, improve this
void AISystem::updateMap() {
    std::lock_guard<std::mutex> lock(mapMtx);
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_Y; ++j) {
            map[i][j] = 1;
        }
    }

    // slot taken by Enemy
    for (Entity aiEntity : manager.getEntitiesWithComponent<AIComponent>()) {   
        int x = manager.getComponent<TransformComponent>(aiEntity).pos.x;
        int y = manager.getComponent<TransformComponent>(aiEntity).pos.y;
        int invertedY = MAX_Y - y;  
        map[x][invertedY] = 0;
    }

    // slot taken by player
    for (Entity playerEntity : manager.getEntitiesWithComponent<PlayerComponent>()) {
        int x = manager.getComponent<TransformComponent>(playerEntity).pos.x;
        int y = manager.getComponent<TransformComponent>(playerEntity).pos.y;
        int invertedY = MAX_Y - y;
        map[x][invertedY] = 0;
    }

    // slot taken by obstacle
    for (Entity obstacleEntity : manager.getEntitiesWithComponent<ObstacleComponent>()) {
        int x = manager.getComponent<TransformComponent>(obstacleEntity).pos.x;
        int y = manager.getComponent<TransformComponent>(obstacleEntity).pos.y;
        int invertedY = MAX_Y - y;
        map[x][invertedY] = 2;
    }

   /* std::cout << std::endl;
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_Y; ++j) {
           
            std::cout << map[i][j] << " ";
        }
        std::cout << std::endl;
        
    }*/
}

void AISystem::processAIEntity(Entity entity) {
    std::lock_guard<std::mutex> lock(mtx);
    auto& aicomponent = manager.getComponent<AIComponent>(entity);
    
    handlePathfindingState(entity);
    handleAttackState(entity);
   
    
}

void AISystem::updateAI(std::vector<Entity>& entities) {
    std::vector<std::thread> threads;

    for (Entity& entity : entities) {
        
        threads.emplace_back(&AISystem::processAIEntity, this, std::ref(entity));
    }

    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

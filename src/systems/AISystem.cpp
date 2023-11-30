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

}

// idle state handler
void AISystem::handleIdleState(Entity entity){
    // swtich to pathfinding when enemy's turn started
    if (isEnemyTurn) {
        std::cout << "Idle state" << std::endl;
        AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
        aiComponent.state = AIState::Pathfinding;
    }
}

// Pathfinding state handler
void AISystem::handlePathfindingState(Entity entity){
    std::cout << "Pathfinding state" << std::endl;
    int map[7][7] = {
    {1,1,1,0,1,1,1 },
    {1,1,1,0,1,1,1 },
    {1,1,1,0,1,1,1 },
    {1,1,1,1,1,2,1 },
    {1,1,1,0,1,1,1 },
    {1,1,1,0,1,1,1 },
    {1,1,1,0,1,1,1 },
    };
    AIComponent& aiComponent = manager.getComponent<AIComponent>(entity);
    EnemyAI aiPath(manager);
    Entity player = aiPath.GetClosestPlayer(entity);
    std::pair aiPosition = std::make_pair(manager.getComponent<GridPositionComponent>(entity).gridX, manager.getComponent<GridPositionComponent>(entity).gridY);
    std::pair playerPosition = std::make_pair(manager.getComponent<GridPositionComponent>(player).gridX, manager.getComponent<GridPositionComponent>(player).gridY);
    // pathfinding
    Pathfinding path;
    path.aStarSearch(map, aiPosition, playerPosition);
    path.printDirVec();
    int walkRange = manager.getComponent<MoveComponent>(entity).moveRange;
    //std::cout << "new position = " << path.getNewPosition(walkRange).first << " : " << path.getNewPosition(walkRange).second << std::endl;
    manager.getComponent<GridPositionComponent>(entity).gridX = path.getNewPosition(walkRange).first;
    manager.getComponent<GridPositionComponent>(entity).gridY = path.getNewPosition(walkRange).second;


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

    // end enemy turn
    isEnemyTurn = false;
    std::cout << "Enemy turn end" << std::endl;
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
    Entity playerEntity = manager.createEntity(); // test

    // register
    manager.registerComponentType<AIComponent>();
    manager.registerComponentType<HealthComponent>();
    manager.registerComponentType<AttackComponent>();
    manager.registerComponentType<MoveComponent>();
    manager.registerComponentType<GridPositionComponent>();

 
    // increment entityID
    ++entityID;

    //// create component
    AIComponent ai(AIState::Idle);
    HealthComponent hp(bus,entityID, 100, 2); // assume health starts at 100 , armor 2
    AttackComponent att(1,1,1); // damage range and attackModifier = 1
    MoveComponent movement(2);
    GridPositionComponent pos(2,2);
    /*TransformComponent trans;
    StaticMeshComponent stat;
    trans.pos = { 1, 1 };
    stat.posOffset.z += 0.6f;
    stat.rotOffset.y = glm::radians(90.0f);
    stat.modelName = "ak";
    stat.textureName = "ak_texture";*/
    GridPositionComponent pos2(4, 4);// test 
    PlayerComponent player(1);// test 


    //// add component
    
    manager.addComponent<AIComponent>(aiEntity, ai);
    manager.addComponent<HealthComponent>(aiEntity, hp);
    manager.addComponent<AttackComponent>(aiEntity, att);
    manager.addComponent<MoveComponent>(aiEntity, movement);
    manager.addComponent<GridPositionComponent>(aiEntity, pos);
    /*manager.addComponent<TransformComponent>(aiEntity, trans);
    manager.addComponent<StaticMeshComponent>(aiEntity, stat);*/

    /*test*/
    manager.addComponent<PlayerComponent>(playerEntity, player);
    manager.addComponent<HealthComponent>(playerEntity, hp);
    manager.addComponent<AttackComponent>(playerEntity, att);
    manager.addComponent<MoveComponent>(playerEntity, movement);
    manager.addComponent<GridPositionComponent>(playerEntity, pos2);
}

// Set isEnemyTurn to allow enemy to start 
void AISystem::startEnemyTurn() {
    isEnemyTurn = true;
}
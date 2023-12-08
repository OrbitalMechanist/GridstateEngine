#include "ecs/entity/EntityManager.h"

// Get timeDiff
float EntityManager::getDeltaTime() {
    // Get the current time
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    // Calculate the time difference between the current frame and the last frame.
    auto deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentFrameTime - lastFrameTime).count();
    // Update the last frame time to the current time for the next call.
    lastFrameTime = currentFrameTime;
    return deltaTime;
}

// EntityManager
EntityManager::EntityManager() : livingEntityCount(0) {
    for (Entity entity = 0; entity < MaxEntities; ++entity) {
        availableEntities.push(entity);
    }
}

// Create Entity
Entity EntityManager::createEntity() {
    Entity id = availableEntities.front(); 
    availableEntities.pop(); // Pop a queue
    ++livingEntityCount;

    return id;
}

// Destroy a Entity
void EntityManager::destroyEntity(Entity entity) {
    // Invalidate the destroyed entity's signature
    signatures[entity].reset();

    // Put the destroyed ID back into the queue
    availableEntities.push(entity);
    --livingEntityCount;
}

// Set a signature
void EntityManager::setSignature(Entity entity, Signature signature) {
    signatures[entity] = signature;
}

// Get a signatrue
Signature EntityManager::getSignature(Entity entity) {
    return signatures[entity];
}



template<typename T>
size_t EntityManager::getComponentTypeID() {
    static size_t typeID = getNextComponentTypeID();
   // std::cout << "Component Type ID for " << typeid(T).name() << ": " << typeID << std::endl;
    return typeID;
}

// Returns the next unique component type ID.
size_t EntityManager::getNextComponentTypeID() {
    static size_t lastID = 0;
    return lastID++;
}

// Get a vector of entities with a specific component
template<typename T>
std::vector<Entity> EntityManager::getEntitiesWithComponent() {
    std::vector<Entity> matchingEntities;
    size_t componentTypeID = getComponentTypeID<T>(); // Get the unique ID for the component type.

    // Iterate over all entities and check their signature
    for (Entity entity = 0; entity < MaxEntities; ++entity) {
        if (signatures[entity].test(componentTypeID)) { 
            matchingEntities.push_back(entity);
        }
    }
    return matchingEntities;
}
template std::vector<Entity> EntityManager::getEntitiesWithComponent<AIComponent>();
template std::vector<Entity> EntityManager::getEntitiesWithComponent<GridPositionComponent>();
template std::vector<Entity> EntityManager::getEntitiesWithComponent<MapComponent>();
template std::vector<Entity> EntityManager::getEntitiesWithComponent<PlayerComponent>();
template std::vector<Entity> EntityManager::getEntitiesWithComponent<MoveComponent>();
template std::vector<Entity> EntityManager::getEntitiesWithComponent<AttackComponent>();
template std::vector<Entity> EntityManager::getEntitiesWithComponent<HealthComponent>();
template std::vector<Entity> EntityManager::getEntitiesWithComponent<ObstacleComponent>();

template std::vector<Entity> EntityManager::getEntitiesWithComponent<TransformComponent>();
template std::vector<Entity> EntityManager::getEntitiesWithComponent<StaticMeshComponent>();

// add a new component type
template<typename T>
void EntityManager::registerComponentType() {
    auto type = std::type_index(typeid(T));
    componentArrays[type] = std::make_unique<ComponentArray<T>>();
}
template void EntityManager::registerComponentType<AIComponent>();
template void EntityManager::registerComponentType<MapComponent>();
template void EntityManager::registerComponentType<GridPositionComponent>();
template void EntityManager::registerComponentType<PlayerComponent>();
template void EntityManager::registerComponentType<MoveComponent>();
template void EntityManager::registerComponentType<AttackComponent>();
template void EntityManager::registerComponentType<HealthComponent>();
template void EntityManager::registerComponentType<ObstacleComponent>();

template void EntityManager::registerComponentType<StaticMeshComponent>();
template void EntityManager::registerComponentType<TransformComponent>();

// getComponent:
template<typename T>
T& EntityManager::getComponent(Entity entity) {
    auto compArrayPtr = std::static_pointer_cast<ComponentArray<T>>(componentArrays[std::type_index(typeid(T))]);
    return compArrayPtr->getComponent(entity);
}
template AIComponent& EntityManager::getComponent<AIComponent>(Entity entity); // explcit for AIComponent
template GridPositionComponent& EntityManager::getComponent<GridPositionComponent>(Entity entity);
template MapComponent& EntityManager::getComponent<MapComponent>(Entity entity);
template PlayerComponent& EntityManager::getComponent<PlayerComponent>(Entity entity);
template MoveComponent& EntityManager::getComponent<MoveComponent>(Entity entity);
template AttackComponent& EntityManager::getComponent<AttackComponent>(Entity entity);
template HealthComponent& EntityManager::getComponent<HealthComponent>(Entity entity);
template ObstacleComponent& EntityManager::getComponent<ObstacleComponent>(Entity entity);

template StaticMeshComponent& EntityManager::getComponent<StaticMeshComponent>(Entity entity);
template TransformComponent& EntityManager::getComponent<TransformComponent>(Entity entity);


template<typename T>
void EntityManager::addComponent(Entity entity, T component) {
    // Get the type index for this component type
    auto typeID = std::type_index(typeid(T));
  
    // Find if there is already a ComponentArray for this component type
    auto it = componentArrays.find(typeID);
    if (it == componentArrays.end()) {
        // If not, create one and add it to the map
        auto newComponentArray = std::make_shared<ComponentArray<T>>();
        componentArrays.insert({ typeID, newComponentArray });
        it = componentArrays.find(typeID); // Re-find the iterator for the newly inserted element
    }

    // Cast the pointer to the correct ComponentArray type and add the component
    std::shared_ptr<ComponentArray<T>> compArray = std::static_pointer_cast<ComponentArray<T>>(it->second);
    compArray->addComponent(entity, component);
    auto componentTypeID = getComponentTypeID<T>(); 

    // Set Signature part
    Signature signature = getSignature(entity); 
    signature.set(componentTypeID);
    setSignature(entity, signature); 
    
}
template void EntityManager::addComponent<AIComponent>(Entity entity, AIComponent aicomponent);
template void EntityManager::addComponent<MapComponent>(Entity entity, MapComponent mapcomponent);
template void EntityManager::addComponent<GridPositionComponent>(Entity entity, GridPositionComponent gridPositioncomponent);
template void EntityManager::addComponent<PlayerComponent>(Entity entity, PlayerComponent playerComponent);
template void EntityManager::addComponent<MoveComponent>(Entity entity, MoveComponent moveComponent);
template void EntityManager::addComponent<AttackComponent>(Entity entity, AttackComponent attackComponent);
template void EntityManager::addComponent<HealthComponent>(Entity entity, HealthComponent healthComponent);
template void EntityManager::addComponent<ObstacleComponent>(Entity entity, ObstacleComponent obstacleComponent);

template void EntityManager::addComponent<TransformComponent>(Entity entity, TransformComponent transformcomponent);
template void EntityManager::addComponent<StaticMeshComponent>(Entity entity, StaticMeshComponent smcomp);


#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <unordered_map>
#include <bitset>
#include <array>
#include <cassert>
#include <queue>
#include <memory>
#include <typeindex>
#include <vector>
#include <iostream>
#include <chrono>
#include <optional>
#include "Entity.h"

// Components
#include "../components/AIComponent.h"
#include "../components/GridPositionComponent.h"
#include "../components/MapComponent.h"


constexpr size_t MaxEntities = 10000; 
constexpr size_t MaxComponents = 32; 
using ComponentType = std::bitset<MaxComponents>;

using Signature = std::bitset<MaxComponents>; 
using Entity = std::uint32_t;

// Forward declaration for the ComponentArray base class
struct IComponentArray {
    virtual ~IComponentArray() = default;
};




class EntityManager {
public:
    EntityManager();
   
    Entity createEntity();
    void destroyEntity(Entity entity);
    void setComponentType(Entity entity, ComponentType type);
    bool hasComponentType(Entity entity, ComponentType type) const;
    ComponentType getComponentType(Entity entity) const;

    float getDeltaTime();

    void setSignature(Entity entity, Signature signature);
    Signature getSignature(Entity entity); // A entity has what components


    // Template method implementations for component handling
    template<typename T> void registerComponentType();
    template<typename T> T& getComponent(Entity entity);
    template<typename T> bool hasComponent(Entity entity) const;
    template<typename T> void addComponent(Entity entity, T component);
    template<typename T> void removeComponent(Entity entity);
    template<typename T> std::vector<Entity> getEntitiesWithComponent();



private:
    // Pool of available entity IDs.
    std::queue<Entity> availableEntities;


    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> componentArrays;

    // Keep track of the number of living entities to limit the creation of new entities.
    size_t livingEntityCount;

    // Array of signatures where the index corresponds to the entity ID.
    std::array<Signature, MaxEntities> signatures;

    // Utility to get the current time difference
    std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();

    template<typename T> size_t getComponentTypeID();
    size_t getNextComponentTypeID();
};

// ComponentArray class
template<typename T>
struct ComponentArray : public IComponentArray {
    std::array<std::optional<T>, MaxEntities> data;

    T& getComponent(Entity entity) {
        return *data[entity];
    }

    void addComponent(Entity entity, T component) {
        data[entity] = component;
    }

    void removeComponent(Entity entity) {
        data[entity].reset();
    }

    bool hasComponent(Entity entity) {
        return data[entity].has_value();
    }
};
#endif

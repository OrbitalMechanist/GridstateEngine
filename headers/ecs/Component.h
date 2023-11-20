#pragma once
#include <vector>
#include "ecs/CommonECS.h"

class GameObject;
enum class ComponentType;

class Component {
protected:
    GameObject& parent;

public:
    Component(GameObject& p, ComponentType t) : parent(p), type(t) {};
    ComponentType type;
    virtual ~Component() = 0;
    virtual void update(float deltaTime) = 0;
private:

};

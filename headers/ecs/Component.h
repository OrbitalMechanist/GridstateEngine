#pragma once
#include <vector>
#include "ecs/CommonECS.h"

class GameObject;

class Component {
protected:
    GameObject& parent;

public:
    Component(GameObject& p) : parent(p) {};
    virtual ~Component() = 0;
    virtual void update(float deltaTime) = 0;
    virtual bool otherComponentInteractionCheck(const std::vector<std::shared_ptr<Component>>& existing) = 0;
};

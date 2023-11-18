#pragma once
#include <vector>
#include <memory>
#include "GameObj.h"

class Component {
protected:
    GameObj& parent;

public:
    Component(GameObj& p) : parent(p) {}
    virtual ~Component() = 0;
    virtual void update(float deltaTime) = 0;
    virtual bool otherComponentInteractionCheck(const std::vector<std::shared_ptr<Component>>& existing) = 0;

    Transform& getParentTransform();
};

#pragma once
#include <vector>

class Component {
public:
    Component(){}
    virtual ~Component() = 0;
    virtual void update(float deltaTime) = 0;
private:

};

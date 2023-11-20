#pragma once

#include <utility>
#include <memory>
#include <string>
#include "ecs/CommonECS.h"

class Component;

class GameObj {
public:
    std::string gameName;

    GameObj(std::string inputName, std::vector<Component*> inputComponents);

	GameObj();

	~GameObj();

    void setSerializedName(std::string input);

	Component* addComponent(Component* c);

	std::vector<Component*> addComponents(std::vector<Component*> input);

	void update(float deltaTime);

	Component* getComponent(Component* c);

	void removeComponent(Component* c);

private:
    std::string serializedName;
	std::vector<Component*> components;
};
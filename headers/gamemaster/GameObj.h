#pragma once

#include <utility>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "Component.h"

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	Transform() : Transform(glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 1,1,1 }) {}
	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) : position(pos), rotation(rot), scale(scl) {}
};

class GameObj {
public:
    std::pair<int, int> gameCoords;
    std::string gameName;
	Transform transform;

    GameObj(std::pair<int, int> inputCoords, std::string inputName);

    void setSerializedName(std::string input);

	Component* addComponent(Component* c);

	void update(float deltaTime);

	Component* getComponent(Component* c);

	void removeComponent(Component* c);

private:
    std::string serializedName;
	std::vector<Component*> components;
};
#include "ecs/GameObj.h"

/*
This is the super class for all GameObjects, it stores game-related values universal to all GameObjects,
such as: its name, its serialized name for the GameObjList, and its components
*/

GameObj::GameObj(std::string inputName, std::vector<Component*> inputComponents) : gameName(inputName), components(inputComponents) {}
void GameObj::setSerializedName(std::string input) { this->serializedName = input; }

Component* GameObj::addComponent(Component* c)
{
	if (getComponent(c))
	{
		return nullptr;
	}
	else {
		components.push_back(std::move(c));
		return components.back();
	}
}

std::vector<Component*> GameObj::addComponents(std::vector<Component*> input) {
	std::vector<Component*> addedComponentsList;
	
	for (unsigned int i = 0; i < input.size(); i++) {
		this->addComponent(input[i]);
		addedComponentsList.push_back(input[i]);
	}

	return addedComponentsList;
}

void GameObj::update(float deltaTime)
{ 
	for (Component* component : components) {
		component->update(deltaTime);
	}
}

Component* GameObj::getComponent(Component* c)
{
	for (Component* component : components) {
		if (component == c) {
			return component;
		}
	}
	return nullptr;
}

void GameObj::removeComponent(Component* c)
{
	std::vector<Component*>::iterator it = components.begin();
	while (it != components.end()) {
		if (*it == c) {
			components.erase(it);
			components.shrink_to_fit();
		}
		++it;
	}
}

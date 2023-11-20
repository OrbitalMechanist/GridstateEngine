#include "ecs/GameObj.h"

/*
This is the super class for all GameObjects, it stores game-related values universal to all GameObjects,
such as: its name, its serialized name for the GameObjList, and its components
*/

GameObj::GameObj(std::string inputName, std::vector<Component*> inputComponents) : gameName(inputName) {}
GameObj::GameObj() {}
GameObj::~GameObj() {
	components.clear();
}

void GameObj::setSerializedName(std::string input) { this->serializedName = input; }
/*
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
}*/

Component* GameObj::addComponent(std::shared_ptr<Component> c) {
	if (c->otherComponentInteractionCheck(components)) {
		return nullptr;
	}
	else {
		components.push_back(std::move(c));
		return components.back().get();
	}
}

void GameObj::update(float deltaTime) {
	for (int i = 0; i < components.size(); ++i) {
		components[i]->update(deltaTime);
	}
}

void GameObj::removeComponent(Component* victim) {
	for (auto i = components.begin(); i < components.end(); ++i) {
		if ((*i).get() == victim) {
			components.erase(i);
			components.shrink_to_fit();
			return;
		}
	}
}

#include "ecs/GameObj.h"

/*
This is the super class for all GameObjects, it stores game-related values universal to all GameObjects,
such as: game-related co-ordinates for use of the MapGrid and later converted to Real-space co-ordinates
for use by the renderer, its in-game name, and a SerializedName given to it by the GameObjList that contains it.
*/

GameObj::GameObj(std::pair<int, int> inputCoords, std::string inputName) : gameCoords(inputCoords), gameName(inputName) {}
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

void GameObj::update(float deltaTime)
{ 
	for (Component* component : components) {
		component->update(deltaTime);
	}
}

Component* GameObj::getComponent(Component* c)
{
	for (Component* component : components) {
		if (component == c); {
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

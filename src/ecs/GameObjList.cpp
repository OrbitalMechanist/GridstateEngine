#include "ecs/GameObjList.h"
#include <iostream>
#include <list>

/*
	This class is a list containing all GameObjects loaded in the current MapGrid
	It is currently updated alongside the MapGrid obj to track every element related to gameplay.

	later the graphics engine will need to append the content of this list to a list of it's own
	in order to render the game world.
*/

GameObjList::GameObjList() {}

// Initialization function, when given a list of GameObjects
GameObjList::GameObjList(std::vector<GameObj> objects) : data(objects) { serializeList(); }

//placeholder adds a GameObj to data
void GameObjList::addGameObj(GameObj obj) {
	this->data.push_back(obj);
	//serializeList() is currently too costlty to do after every added object,
	//once the function is improved it can be called in addGameObj
	//serializeList();
}

//returns # of objects within this GameObjList (use in conjunction with below function to loop
// through the GameObjList outside of the class' scope.
size_t GameObjList::size() {
	return data.size();
}

//grabs an object from data at a specific index (can be used to loop through the GameObjList)
GameObj GameObjList::getGameObj(int index) const {
	if (index < this->data.size()) {
		return this->data[index];
	}
	else {
		//placeholder return null game obj
		return GameObj();
	}
}

//grabs an object from data by its serializedName

//removes an object from data at a specific index
void GameObjList::deleteGameObj(int index) {
	if (index < this->size()) {
		this->data.erase((data.begin() + index));
	}
}

//returns the list of game objects
std::vector<GameObj> GameObjList::getGameObjList() const {
	return this->data;
}

//applies serialized names to all objects
//this implementation is to be improved
void GameObjList::serializeList() {
	for (int i = 0; i < data.size(); i++) {
		std::string serialName = data[i].gameName + std::to_string(i);
		data[i].setSerializedName(serialName);
	}
}
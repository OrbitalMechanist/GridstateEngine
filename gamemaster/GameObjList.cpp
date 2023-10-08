#include "GameObj.cpp"
#include <iostream>
#include <list>
#include <vector>

/*
	This class is a list containing all GameObjects loaded in the current MapGrid
	It is currently updated alongside the MapGrid obj to track every element related to gameplay.

	later the graphics engine will need to append the content of this list to a list of it's own
	in order to render the game world.
*/

class GameObjList {

private:
	std::vector<GameObj> data;

public:
	// Initialization function, empty
	GameObjList() {}

	//placeholder adds a GameObj to data
	void addGameObj(GameObj obj) {
		this->data.push_back(obj);
	}

	//returns # of objects within this GameObjList (use in conjunction with below function to loop
	// through the GameObjList outside of the class' scope.
	int size() {
		return data.size();
	}

	//grabs an object from data at a specific index (can be used to loop through the GameObjList)
	GameObj getGameObj(int index) {
		if (index < this->size()) {
			return this->data[index];
		}
	}

	//removes an object from data at a specific index
	void deleteGameObj(int index) {
		if (index < this->size()) {
			this->data.erase((data.begin() + index));
		}
	}

	//returns the list of game objects
	std::vector<GameObj> getGameObjList() {
		return this->data;
	}
};
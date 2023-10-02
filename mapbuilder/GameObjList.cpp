#include <iostream>
#include <list>
#include <vector>

/*
	This class is a list containing all GameObjects loaded in the current MapGrid
	It is currently updated alongside the MapGrid obj to track every element related to gameplay.

	later the graphics engine will need to append the content of this list to a list of it's own
	in order to render the game world.

	like other placeholder implementation we're using strings until Game Objects are ready
*/

class GameObjList {

private:
	std::vector<std::string> data;

public:
	// Initialization function, empty
	GameObjList() {}

	//placeholder adds a string to data
	void addGameObj(std::string str) {
		this->data.push_back(str);
	}

	//returns # of objects within this GameObjList (use in conjunction with below function to loop
	// through the GameObjList outside of the class' scope.
	int size() {
		return data.size();
	}

	//grabs an object from data at a specific index (can be used to loop through the GameObjList)
	//placeholder returns string
	std::string getGameObj(int index) {
		if (index < this->size()) {
			return this->data[index];
		}
	}

	//removes an object from data at a specific index, placeholder
	void deleteGameObj(int index) {
		if (index < this->size()) {
			this->data[index].erase();
		}
	}
};
#include <iostream>
#include <list>
#include <string>

/*
	This class is a list containing all GameObjects loaded in the current MapGrid
	It is currently updated alongside the MapGrid obj to track every element related to gameplay.

	later the graphics engine will need to append the content of this list to a list of it's own
	in order to render the game world.

	like other placeholder implementation we're using strings until Game Objects are ready
*/

class GameObjList {

private:
	std::list<std::string> data;

public:
	// Initialization function
	GameObjList() {
		// empty
	}
};
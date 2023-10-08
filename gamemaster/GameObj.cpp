#include <utility>
#include <string>

/*
This is the super class for all GameObjects, it stores game-related values universal to all GameObjects,
such as: game-related co-ordinates for use of the MapGrid and later converted to Real-space co-ordinates
for use by the renderer, its in-game name, and a SerializedName given to it by the GameObjList that contains it.
*/

class GameObj {
public:
	std::pair<int, int> gameCoords;
	std::string gameName;

	GameObj(std::pair<int, int> inputCoords, std::string inputName) : gameCoords(inputCoords), gameName(inputName) {}
	void setSerializedName(std::string input) { this->serializedName = input; }

private:
	std::string serializedName;
};
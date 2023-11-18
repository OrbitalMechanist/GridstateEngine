#include "gamemaster/GameObj.h"

/*
This is the super class for all GameObjects, it stores game-related values universal to all GameObjects,
such as: game-related co-ordinates for use of the MapGrid and later converted to Real-space co-ordinates
for use by the renderer, its in-game name, and a SerializedName given to it by the GameObjList that contains it.
*/

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	Transform() : Transform(glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 1,1,1 } {}
	Transform(glm::vec3) (glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) : position(pos), rotationt(rot), scale(scl) {}
};

GameObj::GameObj(std::pair<int, int> inputCoords, std::string inputName) : gameCoords(inputCoords), gameName(inputName) {}
void GameObj::setSerializedName(std::string input) { this->serializedName = input; }

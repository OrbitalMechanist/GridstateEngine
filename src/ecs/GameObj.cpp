#include "ecs/GameObj.h"

/*
This is the super class for all GameObjects, it stores game-related values universal to all GameObjects,
such as: its name, its serialized name for the GameObjList, and its components
*/

GameObj::GameObj(std::string inputName) : gameName(inputName) {}
GameObj::GameObj() {}
GameObj::~GameObj() {}

void GameObj::setSerializedName(std::string input) { this->serializedName = input; }
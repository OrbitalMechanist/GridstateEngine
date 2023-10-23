#include "filePairReader.cpp"
#include "../gamemaster/GameObjList.cpp"
#include "mapgridbuilder/MapGridBuilder.h"
/*
	PLACEHOLDER, this object handles populating MapGrid objects 
	with game objects of many varying types.

	This object will iterate over a .bmp and .xml file pair and
	initialize and populate MapGrid and GameObjList objects with tile information,
	enemy placement, player character placement, and obstacle placement.
*/

//load levels according to a list of level names
std::vector<std::tuple<MapGrid, GameObjList>> MapGridBuilder::loadLevels(std::list<std::string> levelList) {}

//build a level according to given level name by calling functions from the FilePairReader
std::tuple<MapGrid, GameObjList> buildLevel(std::string levelName) {}

//unload the active currentLevel and replace it with a tuple from our preparedLevels grabbed by name
void loadlevel(std::string levelName) {}

//clear preparedLevels and currentLevel
void clear() {}
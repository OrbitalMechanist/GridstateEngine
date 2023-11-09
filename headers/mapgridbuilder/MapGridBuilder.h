#pragma once

#include <utility>
#include <vector>
#include <list>
#include "mapgridbuilder/MapGrid.h"

class MapGridBuilder {
private:
	std::vector<std::tuple<MapGrid, GameObjList>> preparedLevels;
	std::tuple<MapGrid, GameObjList> currentLevel;
public:
	//placeholder returns void, later should return std::vector<std::tuple<MapGrid, GameObjList>> (a vector of MapGrid & GameObjList pairs
	void loadLevels(std::list<std::string> levelList);
	//placeholder returns void, later should return std::tuple<MapGrid, GameObjList> (the MapGrid and GameObjList of the built level)
	void buildLevel(std::string levelName);
	void loadlevel(std::string levelName);
	void clear();
};
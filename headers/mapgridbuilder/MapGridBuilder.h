#include <utility>
#include <vector>

class MapGridBuilder {
private:
	std::vector<std::tuple<MapGrid, GameObjList>> preparedLevels;
	std::tuple<MapGrid, GameObjList> currentLevel;
public:
	std::vector<std::tuple<MapGrid, GameObjList>> loadLevels(std::list<std::string> levelList);
	std::tuple<MapGrid, GameObjList> buildLevel(std::string levelName);
	void loadlevel(std::string levelName);
	void clear();
};
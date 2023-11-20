#pragma once

#include <utility>
#include "ecs/Component.h"

//This component tracks the object's game-related coordinates for pathfinding and targetting

class GameCoords : public Component {
public:
	std::pair<int, int> gameCoords;
};
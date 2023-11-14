#pragma once

#include <utility>
#include <string>

class GameObj {
public:
    std::pair<int, int> gameCoords;
    std::string gameName;

    GameObj(std::pair<int, int> inputCoords, std::string inputName);

    void setSerializedName(std::string input);

private:
    std::string serializedName;
};

